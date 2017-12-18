#ifndef _FINDWORD_
#define _FINDWORD_

/** \file
  \brief Поиск заданного слова в текстовом файле
  
  Объект выполняет функцию поиска заданного слова в текстовом файле.
  Слово должно быть непрерывным, т.е. mother != mo<crlf|tab|space|,|.|:>ther
*/

#include "textbase.h"

#pragma pack(push,1)

///////////////////////////////////////////////////////////////////////////////
/// Поиск слова в текстовом файле и подсчет его вхождений
/// на базе объекта для разбора структурированных текстовых файлов               
class FindWord : public TextBase
{
    char *word_to_find;                ///< Указатель на искомое слово
	unsigned int word_count;           ///< Счетчик вхождений слова

public:

    /// Конструктор
    /// \param [in] char *name имя файла
    /// \param [in] int mode режим открытия файла
    /// \param [in] int access режим доступа к файлу
    FindWord(char *name, int mode=O_RDONLY, int access=0777);

    /// Деструктор
    /// \param нет
    ~FindWord();

    /// Сканирование файла и поиск слова
    /// \param [in] int bufsize размер буфера при сканировании и разборе файла
    /// \param [in] char *to_find указатель на слово для поиска
    /// \return количество вхождений в случае успеха, 0 в случае отсутствия, -1 в случае ошибки (код ошибки в getError())
    int findword(int bufsize, char *to_find);

    /// Разбор строки файла
    /// \param [in] char *str строка файла
    /// \param [in] int32_t pos позиция строки в файле
    /// \param [in] int len длина строки в файле
    /// \return 0 в случае успеха, -1 в случае ошибки, требующей прекращения разбора файла
    virtual int parse(char *str, int32_t pos, int len);

};

#pragma pack(pop)


#endif