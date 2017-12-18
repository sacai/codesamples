#ifndef _CMDLINE_
#define _CMDLINE_

/** \file
  \brief Организация разбора командной строки POSIX-приложения
  
  Данный файл содержит реализацию разбора командной строки POSIX-приложения
*/

#include "appcmdl.h"
#include "textbase.h"

#pragma pack(push,1)

extern "C" {

///////////////////////////////////////////////////////////////////////////////
/// Организация разбора командной строки POSIX-приложения               
class CommandLine : public AppCommandLine
{
    char **args;                     ///< Указатель на массив аргументов
	int fromString;                  ///< Массив аргументов сформирован из строки

public:

    /// Конструктор по умолчанию
    /// \param нет
	CommandLine(void) {Init();};

    /// Конструктор для обработки стандартной командной строки
    /// \param [in] int cnt количество аргументов командной строки
    /// \param [in] char **arg массив аргументов
    /// \param [in] unsigned char s способ представления аргументов в командной строке (по умолчанию UNIX_LIKE)
    CommandLine(int cnt, char **arg, unsigned char s = UNIX_LIKE) {Init(cnt, arg, s);};

    /// Конструктор для обработки командной строки в стиле Windows (аргументы одной сплошной строкой)
    /// \param [in] char *string строка аргументов
    /// \param [in] unsigned char s способ представления аргументов в командной строке
    CommandLine(char *string, unsigned char s = DOS_LIKE) {Init(string, s);};

	// Инициализация по умолчанию
    /// \param нет
	/// \return TRUE (1)
	int Init(void) {args = NULL; fromString = FALSE; return(TRUE);};

    /// Инициализация для обработки стандартной командной строки
    /// \param [in] int cnt количество аргументов командной строки
    /// \param [in] char **arg массив аргументов
    /// \param [in] unsigned char s способ представления аргументов в командной строке
    /// \return 1 в случае успеха, 0 в случае ошибки
	int Init(int cnt, char **arg, unsigned char s = UNIX_LIKE);

    /// Инициализация для обработки командной строки в стиле Windows (аргументы одной сплошной строкой)
    /// \param [in] char *string строка аргументов
    /// \param [in] unsigned char s способ представления аргументов в командной строке
    /// \return 1 в случае успеха, 0 в случае ошибки
	int Init(char *string, unsigned char s = DOS_LIKE);

    /// Деструктор
    /// \param нет
    ~CommandLine() {Close();};

	/// Закрытие
    /// \param нет
    /// \return нет
	void Close(void);

    /// Добавление аргумента в массив аргументов
    /// \param [in] char *argstr строка аргумента
    /// \return 1 в случае успеха, 0 в случае ошибки
    int addArg(char *argstr);

    /// Получить очередной аргумент командной строки
    /// \param [in] int item - порядковый номер агрумента
    /// \return /// указатель на аргумент или NULL в случае неуспеха
    virtual char *getArg(int item) {return(args != NULL && item < count ? args[item] : NULL);};

};

///////////////////////////////////////////////////////////////////////////////
/// Организация разбора файла параметров командной строки
/// на базе объекта для разбора конфигурационных файлов               
class CommandFile : public TextBase
{
    CommandLine *cmd;                ///< Указатель на командную строку

public:

    /// Конструктор
    /// \param [in] char *name имя файла
    /// \param [in] int mode режим открытия файла
    /// \param [in] int access режим доступа к файлу
    CommandFile(char *name, int mode=O_RDONLY, int access=0777);

    /// Деструктор
    /// \param нет
    ~CommandFile();

    /// Сканирование файла параметров
    /// \param [in] int bufsize размер буфера при сканировании и разборе файла
    /// \param [in,out] CommandLine *cml объект командной строки, в которую помещаются параметры
    /// \return 1 в случае успеха, 0 в случае ошибки
    int scan(int bufsize, CommandLine *cml);

    /// Разбор строки файла параметров
    /// \param [in] char *str строка файла параметров
    /// \param [in] int32_t pos позиция строки в файле
    /// \param [in] int len длина строки в файле
    /// \return 0 в случае успеха, -1 в случае ошибки, требующей прекращения разбора файла
    virtual int parse(char *str, int32_t pos, int len);

};

};

#pragma pack(pop)


#endif