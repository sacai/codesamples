#ifndef _CHECKSUM_
#define _CHECKSUM_

/** \file
  \brief Подсчет колнтрольной суммы файла
  
  Объект выполняет функцию подсчета контрольной суммы файла по алгоритму
  checksum = word1 + word2 + ... + wordN,
  где word - 32битное значение из файла
*/

#include "textbase.h"

#pragma pack(push,1)

/// Признак ошибки
#define BAD_CHECKSUM	0xFFFFFFFF

///////////////////////////////////////////////////////////////////////////////
/// Подсчет контрольной суммы файла
/// на базе объекта для разбора текстовых файлов               
class CheckSum : public TextFile
{
	uint32_t check_sum;           ///< Контрольная сумма

public:

    /// Конструктор
    /// \param [in] char *name имя файла
    /// \param [in] int mode режим открытия файла
    /// \param [in] int access режим доступа к файлу
    CheckSum(char *name, int mode=O_RDONLY, int access=0777);

    /// Деструктор
    /// \param нет
    ~CheckSum();

    /// Сканирование файла и подсчет суммы
    /// \param нет
    /// \return контрольная сумма в случае успеха, 0xFFFFFFFF в случае ошибки (код ошибки в getError())
    uint32_t get_checksum();

    /// Разбор блока файла
    /// \param [in] char *str указатель на блок файла
    /// \param [in] int32_t pos позиция строки в файле (игнорируется)
    /// \param [in] int len количество символов в блоке
    /// \return 0 в случае успеха, -1 в случае ошибки, требующей прекращения разбора файла
    virtual int parse(char *str, int32_t pos, int len);

};

#pragma pack(pop)


#endif