#include "sys_mem.h"
#include "cmdline.h"
#include "util.h"

/** @file
  Организация разбора командной строки приложения
*/

///////////////////////////////////////////////////////////////////////////////
/// Инициализация для обработки стандартной командной строки
/// \param int cnt количество аргументов командной строки
/// \param char **arg массив аргументов
/// \param unsigned char s способ представления аргументов в командной строке
/// \return 1 в случае успеха, 0 в случае ошибки
int CommandLine::Init(int cnt, char **arg, unsigned char s)
{
int res;

fromString = FALSE;
setOpt("+");
// Настраиваем количество аргументов,
setCount(cnt);
// их внешнее представление,
setStyle(s);
// и указатель на список
args = NULL;
res = DRTSMallocZero(args, sizeof(char *)*cnt);
for(int i = 0; i < getCount() && res == EOK; i++)
{
	res = DRTSMallocZero(args[i], 1024);
	if(res == EOK)
    	ExpandEnv(arg[i], args[i], 1024);
}
if(res == EOK)
{
	// Ищем в списке аргументов имя файла параметров
	char fileName[MAX_PATH];
	memset(fileName, 0, sizeof(fileName));
	getStringParameter("+", 0, fileName, sizeof(fileName));
	if(strlen(fileName))
	{
    	// Разберем файл параметров
    	CommandFile *file = new CommandFile(fileName);
    	if(file)
    	{
        	int newcount = getCount();
        	if(file->scan(8192, this))
        	{
            	setCount(newcount);
        	}
        	delete_null(file);
    	}
	}
	return(TRUE);
}
return(FALSE);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Инициализация для обработки командной строки Windows (аргументы одной сплошной строкой)
/// \param char *string строка аргументов
/// \param unsigned char s способ представления аргументов в командной строке
/// \return 1 в случае успеха, 0 в случае ошибки
int CommandLine::Init(char *string, unsigned char s)
{
int quote = 0;
int cnt = 0;
char *start = string;
char *end = string;
int res = ENOMEM;

args = NULL;

fromString = TRUE;

// Разбиваем командную строку
for(int i = 0; i < strlen(string); i++)
{
    end = &string[i];
    if(string[i] == '\"')
    {
        if(quote)
            quote = 0;
        else
            quote = 1;
    }
    if(!quote && isspace(string[i]))
    {
        cnt++;
        if(args == NULL)
			res = DRTSMallocZero(args, sizeof(char *)*cnt);
        else
			res = DRTSReallocZero(args, sizeof(char *)*(cnt-1), sizeof(char *)*cnt);
        if(res == EOK)
        {
			res = DRTSMallocZero(args[cnt - 1], end - start + 1);
            if(res == EOK)
            {
                strncpy(args[cnt - 1], start, end - start);
            }
			else
				return(FALSE);
        }
		else
			return(FALSE);
        while(isspace(string[i])) i++;
        start = end = &string[i];
    }
}
end = &string[strlen(string)];
if(start != end)
{
    cnt++;
    if(args == NULL)
		res = DRTSMallocZero(args, sizeof(char *)*cnt);
    else
		res = DRTSReallocZero(args, sizeof(char *)*(cnt-1), sizeof(char *)*cnt);
    if(res == EOK)
    {
		res = DRTSMallocZero(args[cnt - 1], end - start + 1);
        if(res == EOK)
        {
            strncpy(args[cnt - 1], start, end - start);
        }
		else
			return(FALSE);
    }
	else
		return(FALSE);
}
setCount(cnt);

// Настраиваем внешнее представление аргументов,
setStyle(s);

setOpt("+");

// Ищем в списке аргументов имя файла параметров
char fileName[MAX_PATH];
memset(fileName, 0, sizeof(fileName));
getStringParameter("+", 0, fileName, sizeof(fileName));
if(strlen(fileName))
{
    // Разберем файл параметров
    CommandFile *file = new CommandFile(fileName);
    if(file)
    {
        int newcount = getCount();
        if(file->scan(8192, this))
        {
            setCount(newcount);
        }
        delete_null(file);
    }
}
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Закрытие
void CommandLine::Close(void)
{
if(args)
{
    for(int i = 0; i < getCount(); i++)
    {
		DRTSFree(args[i]);
    }
	DRTSFree(args);
}
fromString = FALSE;
setCount(0);
setStyle(0);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Добавление аргумента в массив аргументов
/// \param char *argstr строка аргумента
/// \return 1 в случае успеха, 0 в случае ошибки
int CommandLine::addArg(char *argstr)
{
int res = ENOMEM;
int count = getCount();
count++;
if(args == NULL)
	res = DRTSMallocZero(args, sizeof(char *)*count);
else
	res = DRTSReallocZero(args, sizeof(char *)*(count-1), sizeof(char *)*count);
if(res == EOK)
{
    res = DRTSStrdup(args[count - 1], argstr);
    if(res == EOK)
    {
        setCount(count);
        return 1;
    }
}
return 0;
}
///////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
/// Конструктор класса CommandFile
/// \param char *name имя файла
/// \param int mode режим открытия файла
/// \param int access режим доступа к файлу
CommandFile::CommandFile(char *name, int mode, int access)
         : TextBase(name, mode, access)
{
setDivider("= \t");
setQuoter("\'\"");
cmd = NULL;
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Деструктор класса CommandFile
CommandFile::~CommandFile()
{
cmd = NULL;
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Сканирование файла параметров
/// \param int bufsize размер буфера при сканировании и разборе файла
/// \param CommandLine *cml объект командной строки, в которую помещаются параметры
/// \return 1 в случае успеха, 0 в случае ошибки
int CommandFile::scan(int bufsize, CommandLine *cml)
{
if(cml != NULL)
{
    // Запоминаем текущее количество параметров и указатели на массив
    cmd = cml;
    // Сканируем файл
    if(TextFile::scan(bufsize) == TRUE)
    {
        return 1;
    }
}
setError(errno);
return 0;
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Разбор строки файла параметров
/// \param char *str строка файла параметров
/// \param int32_t pos позиция строки в файле
/// \param int len длина строки в файле
/// \return 0 в случае успеха, -1 в случае ошибки, требующей прекращения разбора файла
int CommandFile::parse(char *string, int32_t pos, int len)
{
int res;
char *str = NULL;

if(string && *string != 0 && !isComment(*string))
{
	res = DRTSMallocZero(str, 1024);
	if(res != EOK)
    {
        setError(errno);
        return -1;
    }
    char *field = strtok(string);
    while(field)
    {
        field = Trim(field);
        ExpandEnv(field, str, 1024);
        if(cmd)
        {
            if(!cmd->addArg(str))
            {
                setError(errno);
                DRTSFree(str);
                return -1;
            }
        }
        field = strtok(NULL);
    }
    DRTSFree(str);
}
return 0;
}
///////////////////////////////////////////////////////////////////////////////

