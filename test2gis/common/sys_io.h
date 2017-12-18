#ifndef __SYS_IO__
#define __SYS_IO__

//////////////////////////////////////////////////////////////////////////////
/** \file
 	\brief Функции чтения/записи в файл, с контролем прерывания по сигналу
 	
 	Функции чтения/записи в файл, с контролем прерывания по сигналу и возвратом в цикл открытия/чтения/записи
 */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "sys_defs.h"

#pragma pack(push,1)

#ifndef O_BINARY
#define O_BINARY	0
#endif

#ifndef O_TEXT
#define O_TEXT		0
#endif

extern "C"

{

/// Открытие файла с контролем прерывания по сигналу
/// \param [out] int *handle - указатель на переменную под дескриптор файла
/// \param [in] char *name - имя файла
/// \param [in] int mode - режим открытия файла
/// \param [in] int access - маска прав на доступ к файлу
/// \return EOK (0) в случае успеха
/// \return EINVAL (22), если указатели не инициализированы
/// \return ошибку открытия
int open_r(int *handle, char *name, int mode, int access = 0777);

/// Чтение файла с контролем прерывания по сигналу
/// \param [in] int handle - дескриптор файла
/// \param [out] void *buffer - указатель на буфер для чтения из файл
/// \param [in] size_t size - размер буфера для записи в файл
/// \param [out] off_t *r_bytes - указатель на переменную под количество прочитанных байт
/// \return EOK (0) в случае успеха
/// \return ENOENT (2), если файл не открыт
/// \return EINVAL (22), если указатель на буфер не инициализирован
/// \return ошибку чтения, если не удалось прочитать необходимое количество данных, и при этом чтение не прерывалось сигналом
int read_r(int handle, void *buffer, size_t size, off_t *r_bytes = NULL);

/// Запись файла с контролем прерывания по сигналу
/// \param [in] int handle - дескриптор файла
/// \param [in] void *buffer - указатель на буфер для записи в файл
/// \param [in] size_t size - размер буфера для записи в файл
/// \param [out] off_t *w_bytes - указатель на переменную под количество записанных байт
/// \return EOK (0) в случае успеха
/// \return ENOENT (2), если файл не открыт
/// \return EINVAL (22), если указатель на буфер не инициализирован
/// \return ошибку записи, если не удалось записать необходимое количество данных, и при этом чтение не прерывалось сигналом
int write_r(int handle, void *buffer, size_t size, off_t *w_bytes = NULL);

/// Закрытие файла с контролем прерывания по сигналу
/// \param [in] int handle - дескриптор файла
/// \return EOK (0) в случае успеха
/// \return ошибку закрытия
int close_r(int handle);

};

#pragma pack(pop)

#endif