
/** \file
	\brief Интерфейс для разбора текстового файла на строки, разделенные парой CR-LF (0x0d-0x0a)
	
	Базовый класс TextFile предоставляет функции открытия, закрытия и разбора текстовых файлов на строки, с возможностью
	переопределить в производном классе функцию разбора каждой строки TextFile::parse

*/

#include "sys_defs.h"
#include <string.h>
#include <sys/stat.h>
#include "sys_io.h"
#include "sys_mem.h"
#include "textfile.h"
#include "util.h"

/// Конструктор по умолчанию
TextFile::TextFile()
{
flag=0;
filename = NULL;
handle = -1;
tbuf = NULL;
flength = 0;
}

/// Конструктор
/// \param [in] char *fname имя файла
/// \param [in] int mode режим открытия файла, по умолчанию только для чтения
/// \param [in] int access режим доступа к файлу, если он создается, по умолчанию полный доступ
TextFile::TextFile(char *fName, int mode, int access)
{
flag=0;
tbuf = NULL;
filename = NULL;
flength = 0;
open(fName, mode, access);
}

/// Открытие файла
/// \param [in] char *fname имя файла
/// \param [in] int mode режим открытия файла, по умолчанию только для чтения
/// \param [in] int access режим доступа к файлу, если он создается, по умолчанию полный доступ
/// \return дескриптор открытого файла, либо -1 в случае ошибки
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

/// Закрытие файла
/// \param [in] int remfile удалять ли файл при закрытии
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

/// Сканирование файла
/// \param [in] unsigned bufsize размер буфера при сканировании
/// \param [in] int32_t fromwhere позиция файла, с которой должно начаться сканирование
/// \param [in] bool split разбивать (true) или нет (false) файл на строки, в последнем случае простое поблочное чтение
/// \return 0, если разбор завершился успешно, -1 в случае ошибки либо досрочного прерывания сканирования
int TextFile::scan(unsigned bufsize, int32_t fromwhere, bool split)
{
if(handle==-1) return -1;

DRTSFree(tbuf);
int res = DRTSMallocZero(tbuf, bufsize);   // Буфер для блока
if(res != EOK)
{
  setError(res);
  return -1;
}

char *block_begin;    // Указатель на начало блока
char *string_begin;   // Указатель на начало строки в блоке
char *string_end;     // Указатель на конец строки в блоке


int offset=0;                // Длина оставшегося куска блока
int32_t curstrpos=fromwhere;    // Текущая позиция строки в файле

int nbytesread=0;     // Количество байт, считанных из файла
int32_t filelen;
int len=0;            //  Длина строки
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
			len=(string_end-string_begin)+1;      // длина строки
			offset-=len;                       // Количество байт до конца буфера
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

