#include "findword.h"
#include "util.h"

/** @file
  \brief Поиск заданного слова в текстовом файле
  
  Объект выполняет функцию поиска заданного слова в текстовом файле и подсчета вхожданий слова.
  Слово должно быть непрерывным, т.е. mother != mo<crlf|tab|space|,|.|:>ther
*/

//////////////////////////////////////////////////////////////////////////////
/// Конструктор
/// \param char *name имя файла
/// \param int mode режим открытия файла
/// \param int access режим доступа к файлу
FindWord::FindWord(char *name, int mode, int access)
         : TextBase(name, mode, access)
{
setDivider("=:;,. \t");
setQuoter("\'\"");
word_to_find = NULL;
word_count = 0;
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Деструктор
FindWord::~FindWord()
{
word_to_find = NULL;
word_count = 0;
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Сканирование файла и поиск слова
/// \param [in] int bufsize размер буфера при сканировании и разборе файла
/// \param [in] char *to_find указатель на слово для поиска
/// \return количество вхождений в случае успеха, 0 в случае отсутствия, -1 в случае ошибки (код ошибки в getError())
int FindWord::findword(int bufsize, char *to_find)
{
if(to_find != NULL)
{
    word_to_find = to_find;
    word_count = 0;
    // Сканируем файл
    if(TextFile::scan(bufsize) != -1)
    {
        return word_count;
    }
	else if(getError() == EOK)
		return 0;
}
else
	setError(EINVAL);
return -1;
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Разбор строки файла
/// \param char *str строка файла параметров
/// \param int32_t pos позиция строки в файле
/// \param int len длина строки в файле
/// \return 0 в случае успеха, -1 в случае ошибки, требующей прекращения разбора файла
int FindWord::parse(char *str, int32_t pos, int len)
{
if(str && *str != 0 && !isComment(*str))
{
    char *field = strtok(str);
    while(field)
    {
		if(!strcmp(field, word_to_find))
			word_count++;
        field = strtok(NULL);
    }
}
return 0;
}
///////////////////////////////////////////////////////////////////////////////

