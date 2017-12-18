/** \file
	\brief	Различные сервисные функции для использования как внутри библиотеки, так и в конкретных задачах

*/

#include "sys_defs.h"
#include <dirent.h>
#include <sched.h>
#include <limits.h>
#include "sys_mem.h"
#include "sys_io.h"
#include "util.h"

///////////////////////////////////////////////////////////////////////////////
/// Обрезание пробелов в начале и конце строки
/// \param [in, out] char *string - строка для обработки
/// \return указатель на строку без пробелов в начале и в конце
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
/// Возвращает имя файла без пути
/// \param [in] char *Path - полный путь и имя файла
/// \return указатель на имя файла
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
/// Подстановка переменных окружения в строку
/// \param [in]
/// char *src - строка, содержащая переменные окружения в виде $переменная
/// \param [out]
/// char *dst - буфер для строки, расширенной значениями переменных окружения
/// \param [in]
/// int dstSize - размер буфера в байтах
/// \param [in]
/// char *quotestr - строка символов. который могут быть использованы в качестве
///                  ограничителей, по умолчанию используется символ '$'
/// \return длина строки-результата, либо 0 в случае ошибки
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
/// Добавляет слэш (\ или /) в строку пути файловой системы
/// \param [in, out] char *Path - строка пути
/// \param [in] int reverse - прямой (0) или обратный (1) слэш
/// \return нет
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
/// Получение размера файла по дескриптору (с использованием lseek)
/// \param [in]
/// int handle - дескриптор файла
/// \return
/// размер файла, либо -1 в случае ошибки
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
/// Получение размера файла по дескриптору (с использованием lseek) с возвратом признака ошибки
/// \param [in]
/// int handle - дескриптор файла
/// \param [out]
/// off_t *size - размер файла
/// \return
/// EOK (0), либо признак ошибки
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
/// Получение размера файла по имени (с использованием lseek)
/// \param [in]
/// char *name - имя файла
/// \return
/// размер файла, либо -1 в случае ошибки
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
/// Получение размера файла по имени (с использованием lseek) с возвратом признака ошибки
/// \param
/// char *name - имя файла
/// \param [out]
/// off_t *size - размер файла
/// \return
/// EOK (0), либо признак ошибки
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
/// Сравнение строк без учета регистра
/// \param [in] const char *str1 - левая часть неравенства
/// \param [in] const char *str2 - правая часть неравенства
/// \return < 0, если str1 < str2
/// \return 0, если str1 == str2
/// \return > 0, если str1 > str2
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
/// Сравнение строк без учета регистра не более чем заданного количества символов
/// \param [in] const char *str1 - левая часть неравенства
/// \param [in] const char *str2 - правая часть неравенства
/// \param [in] size_t count - количество сравниваемых символов
/// \return < 0, если str1 < str2
/// \return 0, если str1 == str2
/// \return > 0, если str1 > str2
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
