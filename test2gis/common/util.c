/** \file
	\brief	������� �ࢨ�� �㭪樨 ��� �ᯮ�짮����� ��� ����� ������⥪�, ⠪ � � �������� ������

*/

#include "sys_defs.h"
#include <dirent.h>
#include <sched.h>
#include <limits.h>
#include "sys_mem.h"
#include "sys_io.h"
#include "util.h"

///////////////////////////////////////////////////////////////////////////////
/// ��१���� �஡���� � ��砫� � ���� ��ப�
/// \param [in, out] char *string - ��ப� ��� ��ࠡ�⪨
/// \return 㪠��⥫� �� ��ப� ��� �஡���� � ��砫� � � ����
char *Trim(char *string)
{
while(isspace(*string)) string++;
char *cp = strrchr(string,0);
if(cp && cp != string)
{
    cp--;
    while(isspace(*cp))
    {
        *cp = 0;
        cp--;
    }
}
return(string);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// �����頥� ��� 䠩�� ��� ���
/// \param [in] char *Path - ����� ���� � ��� 䠩��
/// \return 㪠��⥫� �� ��� 䠩��
char* PointToName(char *Path)
{
  char *NamePtr=Path;
  while (*Path)
  {
    if (*Path=='\\' || *Path=='/' || *Path==':')
      NamePtr=Path+1;
    Path++;
  }
  return(NamePtr);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// ����⠭���� ��६����� ���㦥��� � ��ப�
/// \param [in]
/// char *src - ��ப�, ᮤ�ঠ�� ��६���� ���㦥��� � ���� $��६�����
/// \param [out]
/// char *dst - ���� ��� ��ப�, ���७��� ���祭�ﬨ ��६����� ���㦥���
/// \param [in]
/// int dstSize - ࠧ��� ���� � �����
/// \param [in]
/// char *quotestr - ��ப� ᨬ�����. ����� ����� ���� �ᯮ�짮���� � ����⢥
///                  ��࠭��⥫��, �� 㬮�砭�� �ᯮ������ ᨬ��� '$'
/// \return ����� ��ப�-१����, ���� 0 � ��砥 �訡��
int ExpandEnv(char *src, char *dst, int dstSize, char *quotestr)
{
if(src == NULL || dst == NULL)
	return 0;
int i = 0, beginVar = 0;
char varbuf[MAX_PATH];
char *value = NULL;
char *dstPtr = dst;
char *varPtr = varbuf;
char quotechar = 0;
memset(dst, 0, dstSize);
memset(varbuf, 0, sizeof(varbuf));
if(quotestr == NULL)
	quotestr = (char *)"$";
while(*src && strlen(dst) < dstSize-1)
{
    if(memchr(quotestr, *src, strlen(quotestr))!=NULL)
    {
        quotechar = *src;
        beginVar = 1;
    }
    else
    {
        if(beginVar)
        {
            strncpy(varbuf, src, sizeof(varbuf) - 1);
            i = strlen(varbuf);
            while(strlen(varbuf))
            {
                value = getenv(varbuf);
                if(value)
                {
                    strncpy(dstPtr, value, dstSize-1);
                    dstPtr += strlen(dstPtr);
                    src += strlen(varbuf) - 1;
                    break;
                }
                else
                    varbuf[i] = 0;
                i--;
            }
            if(strlen(varbuf) == 0)
            {
                *dstPtr = quotechar;
                dstPtr++;
                *dstPtr = *src;
                dstPtr++;
            }
            memset(varbuf, 0, sizeof(varbuf));
            beginVar = 0;
        }
        else
        {
            *dstPtr = *src;
            dstPtr++;
        }
    }
    src++;
}
return(strlen(dst));
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// �������� ��� (\ ��� /) � ��ப� ��� 䠩����� ��⥬�
/// \param [in, out] char *Path - ��ப� ���
/// \param [in] int reverse - ��אַ� (0) ��� ����� (1) ���
/// \return ���
void AddEndSlash(char *Path, int reverse)
{
int Length=strlen(Path);
if (Length==0 || (Path[Length-1]!='/' && Path[Length-1]!='\\'))
{
	if(reverse)
		strcat(Path,"\\");
	else
		strcat(Path,"/");
}
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// ����祭�� ࠧ��� 䠩�� �� ���ਯ��� (� �ᯮ�짮������ lseek)
/// \param [in]
/// int handle - ���ਯ�� 䠩��
/// \return
/// ࠧ��� 䠩��, ���� -1 � ��砥 �訡��
off_t GetFileSizeByHandle(int handle)
{
off_t result = -1;
off_t curpos = 0;

if(handle != -1)
{
	curpos = lseek(handle, 0, SEEK_CUR);
	if(curpos != -1)
	{
		result = lseek(handle, 0, SEEK_END);
		if(result != -1)
			lseek(handle, curpos, SEEK_SET);
	}
}

return(result);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// ����祭�� ࠧ��� 䠩�� �� ���ਯ��� (� �ᯮ�짮������ lseek) � �����⮬ �ਧ���� �訡��
/// \param [in]
/// int handle - ���ਯ�� 䠩��
/// \param [out]
/// off_t *size - ࠧ��� 䠩��
/// \return
/// EOK (0), ���� �ਧ��� �訡��
int GetFileSizeByHandleEx(int handle, off_t *size)
{
int result = EINVAL;
off_t curpos = 0, sz = 0;

if(handle != -1 && size)
{
	curpos = lseek(handle, 0, SEEK_CUR);
	if(curpos != -1)
	{
		sz = lseek(handle, 0, SEEK_END);
		if(sz != -1)
		{
			lseek(handle, curpos, SEEK_SET);
			*size = sz;
			result = EOK;
		}
		else
			result = errno;
	}
	else
		result = errno;
}

return(result);
}
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// ����祭�� ࠧ��� 䠩�� �� ����� (� �ᯮ�짮������ lseek)
/// \param [in]
/// char *name - ��� 䠩��
/// \return
/// ࠧ��� 䠩��, ���� -1 � ��砥 �訡��
off_t GetFileSizeByName(char *name)
{
off_t result = -1;
off_t curpos = 0;
int handle, res;

if(name)
{
	res = open_r(&handle, name, O_RDONLY | O_BINARY);
	if(result == EOK)
	{
		res = GetFileSizeByHandleEx(handle, &result);
		close_r(handle);
	}
}

return(result);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// ����祭�� ࠧ��� 䠩�� �� ����� (� �ᯮ�짮������ lseek) � �����⮬ �ਧ���� �訡��
/// \param
/// char *name - ��� 䠩��
/// \param [out]
/// off_t *size - ࠧ��� 䠩��
/// \return
/// EOK (0), ���� �ਧ��� �訡��
int GetFileSizeByNameEx(char *name, off_t *size)
{
off_t result = EINVAL;
off_t curpos = 0, sz = 0;
int handle;

if(name && size)
{
	result = open_r(&handle, name, O_RDONLY | O_BINARY);
	if(result == EOK)
	{
		result = GetFileSizeByHandleEx(handle, &sz);
		close_r(handle);
	}
}

return(result);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// �ࠢ����� ��ப ��� ��� ॣ����
/// \param [in] const char *str1 - ����� ���� ��ࠢ���⢠
/// \param [in] const char *str2 - �ࠢ�� ���� ��ࠢ���⢠
/// \return < 0, �᫨ str1 < str2
/// \return 0, �᫨ str1 == str2
/// \return > 0, �᫨ str1 > str2
int Stricmp(const char *str1, const char *str2)
{
int result = 0;

if(!str1 && str2)
	result = -EINVAL;
else if(str1 && !str2)
	result = EINVAL;
else
{
#if defined(__QNX__) || defined(_MSC_VER)
    result = stricmp(str1, str2);
#elif defined (__GNUC__)
    result = strcasecmp(str1, str2);
#endif
}

return(result);
}

///////////////////////////////////////////////////////////////////////////////
/// �ࠢ����� ��ப ��� ��� ॣ���� �� ����� 祬 ��������� ������⢠ ᨬ�����
/// \param [in] const char *str1 - ����� ���� ��ࠢ���⢠
/// \param [in] const char *str2 - �ࠢ�� ���� ��ࠢ���⢠
/// \param [in] size_t count - ������⢮ �ࠢ�������� ᨬ�����
/// \return < 0, �᫨ str1 < str2
/// \return 0, �᫨ str1 == str2
/// \return > 0, �᫨ str1 > str2
int Strnicmp(const char *str1, const char *str2, size_t count)
{
int result = 0;

if(!str1 && str2)
	result = -EINVAL;
else if(str1 && !str2)
	result = EINVAL;
else
{
#if defined(__QNX__) || defined(_MSC_VER)
    result = strnicmp(str1, str2, count);
#elif defined (__GNUC__)
    result = strncasecmp(str1, str2, count);
#endif
}

return(result);
}
