
/** \file
	\brief ����䥩� ��� ࠧ��� ⥪�⮢��� 䠩�� �� ��ப�, ࠧ������� ��ன CR-LF (0x0d-0x0a)
	
	������ ����� TextFile �।��⠢��� �㭪樨 ������, ������� � ࠧ��� ⥪�⮢�� 䠩��� �� ��ப�, � ������������
	��८�।����� � �ந������� ����� �㭪�� ࠧ��� ������ ��ப� TextFile::parse

*/

#include "sys_defs.h"
#include <string.h>
#include <sys/stat.h>
#include "sys_io.h"
#include "sys_mem.h"
#include "textfile.h"
#include "util.h"

/// ��������� �� 㬮�砭��
TextFile::TextFile()
{
flag=0;
filename = NULL;
handle = -1;
tbuf = NULL;
flength = 0;
}

/// ���������
/// \param [in] char *fname ��� 䠩��
/// \param [in] int mode ०�� ������ 䠩��, �� 㬮�砭�� ⮫쪮 ��� �⥭��
/// \param [in] int access ०�� ����㯠 � 䠩��, �᫨ �� ᮧ������, �� 㬮�砭�� ����� �����
TextFile::TextFile(char *fName, int mode, int access)
{
flag=0;
tbuf = NULL;
filename = NULL;
flength = 0;
open(fName, mode, access);
}

/// ����⨥ 䠩��
/// \param [in] char *fname ��� 䠩��
/// \param [in] int mode ०�� ������ 䠩��, �� 㬮�砭�� ⮫쪮 ��� �⥭��
/// \param [in] int access ०�� ����㯠 � 䠩��, �᫨ �� ᮧ������, �� 㬮�砭�� ����� �����
/// \return ���ਯ�� ����⮣� 䠩��, ���� -1 � ��砥 �訡��
int TextFile::open(char *fName, int mode, int access)
{
int res = 0;

DRTSFree(tbuf);
if(fName != NULL && *fName != 0)
{
    res=open_r(&handle, fName, mode, access);
    if(res != EOK)
		setError(res);
	else
	{
    	flength = GetFileSizeByHandle(handle);
 		DRTSFree(filename);
 		res = DRTSStrdup(filename, fName);
 		setError(res);
	}
}
else
     setError(ENOENT);
return(handle);
}

/// �����⨥ 䠩��
/// \param [in] int remfile 㤠���� �� 䠩� �� �����⨨
void TextFile::close(int remfile)
{
if(handle!=-1)
{
	close_r(handle);
	if(remfile && filename)
    	unlink(filename);
	DRTSFree(filename);
	handle = -1;
}
DRTSFree(tbuf);
}

/// �����஢���� 䠩��
/// \param [in] unsigned bufsize ࠧ��� ���� �� ᪠��஢����
/// \param [in] int32_t fromwhere ������ 䠩��, � ���ன ������ ������� ᪠��஢����
/// \param [in] bool split ࠧ������ (true) ��� ��� (false) 䠩� �� ��ப�, � ��᫥���� ��砥 ���⮥ �����筮� �⥭��
/// \return 0, �᫨ ࠧ��� �����訫�� �ᯥ譮, -1 � ��砥 �訡�� ���� ����筮�� ���뢠��� ᪠��஢����
int TextFile::scan(unsigned bufsize, int32_t fromwhere, bool split)
{
if(handle==-1) return -1;

DRTSFree(tbuf);
int res = DRTSMallocZero(tbuf, bufsize);   // ���� ��� �����
if(res != EOK)
{
  setError(res);
  return -1;
}

char *block_begin;    // �����⥫� �� ��砫� �����
char *string_begin;   // �����⥫� �� ��砫� ��ப� � �����
char *string_end;     // �����⥫� �� ����� ��ப� � �����


int offset=0;                // ����� ��⠢襣��� ��᪠ �����
int32_t curstrpos=fromwhere;    // ������ ������ ��ப� � 䠩��

int nbytesread=0;     // ������⢮ ����, ��⠭��� �� 䠩��
int32_t filelen;
int len=0;            //  ����� ��ப�
filelen=flength-fromwhere;
lseek(handle,fromwhere,SEEK_SET);

block_begin=tbuf;

while((nbytesread=read(handle,block_begin,bufsize-offset))>0)
{
	string_begin=tbuf;

	if(split)
	{
		offset+=nbytesread;
		while((string_end=(char *)memchr(string_begin,'\n',offset))!=NULL)
		{
			len=(string_end-string_begin)+1;      // ����� ��ப�
			offset-=len;                       // ������⢮ ���� �� ���� ����
			*string_end='\0';
			if(*(string_end-1)=='\r')
				*(string_end-1)='\0';
			if((parse(string_begin,curstrpos,len))==-1)
			{
				DRTSFree(tbuf);
				return -1;
			}
			curstrpos+=len;
			string_begin=string_end+1;
		}
		memmove(tbuf,string_begin,offset);
		block_begin=tbuf+offset;
	}
	else
	{
		if((parse(string_begin,curstrpos,nbytesread))==-1)
		{
			DRTSFree(tbuf);
			return -1;
		}
		curstrpos+=nbytesread;
	}
}

if(offset)
{
	tbuf[offset]='\0';
	parse(tbuf,curstrpos,offset);
}
parse((char *)"",flength,0);
DRTSFree(tbuf);
return 0;
}

