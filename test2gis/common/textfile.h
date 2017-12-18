#if !defined(__TEXTFILE_H)
#define __TEXTFILE_H

/** \file
	\brief Интерфейс для разбора текстового файла на строки, разделенные парой CR-LF (0x0d-0x0a)
	
	Базовый класс TextFile предоставляет функции открытия, закрытия и разбора текстовых файлов на строки, с возможностью
	переопределить в производном классе функцию разбора каждой строки TextFile::parse

*/


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "sys_base.h"

#pragma pack(push,1)

extern "C" {

/// Базовый класс TextFile предоставляет функции открытия, закрытия и разбора текстовых файлов на строки, с возможностью
/// переопределить в производном классе функцию разбора каждой строки TextFile::parse
class TextFile : public virtual DRTS_Base_Object
{
    char *tbuf;	///< Указатель на буфер
protected:

    int flag;	///< Флаг
	uint32_t flength; ///< размер файла
public:
    char *filename;	///< Имя файла
    int handle;		///< Дескриптор файла

    /// Конструктор по умолчанию
    TextFile();

    /// Конструктор
    /// \param [in] char *fname имя файла
    /// \param [in] int mode режим открытия файла, по умолчанию только для чтения
    /// \param [in] int access режим доступа к файлу, если он создается, по умолчанию полный доступ
    TextFile(char *fName, int mode=O_RDONLY, int access=0777);

    /// Деструктор
    ~TextFile()  {close();};
    
    /// Открытие файла
    /// \param [in] char *fname имя файла
    /// \param [in] int mode режим открытия файла, по умолчанию только для чтения
    /// \param [in] int access режим доступа к файлу, если он создается, по умолчанию полный доступ
    /// \return дескриптор открытого файла, либо -1 в случае ошибки
    int open(char *fName, int mode=O_RDONLY, int access=0777);

    /// Закрытие файла
    /// \param [in] int remfile удалять ли файл при закрытии
    void close(int remfile = 0);

    /// Сканирование файла
    /// \param [in] unsigned bufsize размер буфера при сканировании
    /// \param [in] int32_t fromwhere позиция файла, с которой должно начаться сканирование
	/// \param [in] bool split разбивать (true) или нет (false) файл на строки, в последнем случае простое поблочное чтение
    /// \return 0, если разбор завершился успешно, -1 в случае ошибки либо досрочного прерывания сканирования
    int scan(unsigned bufsize=512, int32_t fromwhere=0L, bool split=true);

    /// Функция разбора строки. Определяется в производном классе.
    /// \param [in] char *str строка для разбора
    /// \param [in] int32_t pos позиция строки в файле
    /// \param [in] int len длина строки
    /// \return 0, если разбор завершился успешно, -1 в случае ошибки либо необходимости досрочного прерывания сканирования
    virtual int parse(char *str, int32_t pos, int len) {return 0;};

};

};

#pragma pack(pop)

#endif