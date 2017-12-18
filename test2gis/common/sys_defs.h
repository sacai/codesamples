#ifndef __SYS_DEFINITIONS__
#define __SYS_DEFINITIONS__

#if defined(__QNXNTO__)
#if !defined(__EXT_POSIX1_199309)
#define __EXT_POSIX1_199309
#endif
#if !defined(__EXT_POSIX1_198808)
#define __EXT_POSIX1_198808
#endif
#if !defined(_QNX_SOURCE)
#define _QNX_SOURCE
#endif
#endif

#include <stddef.h>
#include <stdint.h>
#include <errno.h>

/** 
\file

\brief Заголовочный файл с описанием основных структур

Данный файл содержит в себе определения основных
структур, функций и констант, используемых в системе

*/

#pragma pack(push,1)

/// \defgroup log_returns Константы для определения возвращаемых логических значений
/// @{
#ifndef FALSE
#define FALSE 0		///< Ложь
#endif
#ifndef TRUE
#define TRUE  1		///< Истина
#endif
#ifndef NEGATIVE
#define NEGATIVE -1		///< Признак ошибки
#endif
/// @}

#ifndef EZERO
#define EZERO 0		///< Определение признака отсутствия ошибки (errno == 0)
#endif
#ifndef EOK
#define EOK EZERO	///< Переопределение признака отсутствия ошибки EOK, если он не определен в стандартном заголовке
#endif

#ifndef __Byte_t__
#define __Byte_t__
/// \defgroup byte_t Определение байтового типа
/// @{

/// Байтовый тип (не во всех языках есть в явном виде)
typedef uint8_t byte_t;

#define MAX_BYTE_T	255		///< Максимальное значение целого числа байтового типа

/// @}
#endif

#ifndef ___bit_ops
#define ___bit_ops
/// \defgroup bit_ops Набор битовых операций
/// @{

/// Проверка состояния бита в наборе
/// \param [in,out] [in] - набор битов
/// \param [in] bit - номер бита в наборе, считая с 0
/// \return 0, если бит не взведен, не 0, если бит взведен
#define check_bit(in, bit)   (in & (1 << (bit)))

/// Установка бита в наборе в единицу
/// \param [in,out] [in] - набор битов
/// \param [in] bit - номер бита в наборе, считая с 0
/// \return нет
#define set_bit(in, bit)   (in | (1 << (bit)))

/// Сброс бита в наборе в нуль
/// \param [in,out] [in] - набор битов
/// \param [in] bit - номер бита в наборе, считая с 0
/// \return нет
#define clr_bit(in, bit)   (in & ~(1 << (bit)))

/// Инвертирование бита в наборе
/// \param [in,out] [in] - набор битов
/// \param [in] bit - номер бита в наборе, считая с 0
/// \return нет
#define inv_bit(in, bit)   (in ^ (1 << (bit)))

/// Проверка набора битов по маске
/// \param [in,out] [in] - набор битов
/// \param [in] mask - битовая маска
/// \return 0, если набор битов не соответствует маске, не 0, если соответствует
#define check_mask(in, mask)   (in & (mask))

/// Установка битов в наборе по маске
/// \param [in,out] [in] - набор битов
/// \param [in] mask - битовая маска
/// \return нет
#define set_mask(in, mask)   (in | (mask))

/// Сброс битов в наборе по маске
/// \param [in,out] [in] - набор битов
/// \param [in] mask - битовая маска
/// \return нет
#define clr_mask(in, mask)   (in & ~(mask))

/// @}
#endif



#ifndef MAX_PATH
#define MAX_PATH         260       ///< максимальный размер имени файла
#endif

#ifndef MAX_STRING
#define MAX_STRING       80        ///< максимальный размер абстрактной строки
#endif

#ifndef MAX_SHORT_STRING
#define MAX_SHORT_STRING 32        ///< максимальный размер абстрактной строки
#endif

#ifndef MAX_BUFFER_STRING
#define MAX_BUFFER_STRING 1024     ///< максимальный размер строки, выделяемой под буфер
#endif

#ifndef MAX_BUFFER_SIZE
#define MAX_BUFFER_SIZE 8192       ///< максимальный размер абстрактного буфера
#endif

/// \defgroup millisec Количественные константы
/// @{
#define MILLISEC     1000           ///< количество миллисекунд в секунде
#define MICROSEC     1000000        ///< количество микросекунд в секунде
#define NANOSEC      1000000000     ///< количество наносекунд в секунде
#define NANOMILLISEC 1000000        ///< количество наносекунд в миллисекунде
#define NANOMICROSEC 1000        ///< количество наносекунд в микросекунде

#define M2U(ms)		(ms * 1000)     ///< перевод миллисекунд в микросекунды

#define N2U(ns)		(ns / 1000)     ///< перевод наносекунд в микросекунды

/// @}

/// \defgroup err_codes Дополнительные коды ошибок
/// @{
#define __ETIMEOUT -4            ///< Истечение интервала ожидания
/// @}

/// \defgroup null_macro Макросы для работы с указателями
/// @{
#ifndef delete_null
/// Освободить память оператором delete с проверкой указателя на NULL
/// \param [in,out] item - указатель на объект
/// \return нет
#define delete_null(item)    {if((item) != NULL) delete (item); (item) = NULL;};
#endif

#ifndef free_null
/// Освободить память функцией free с проверкой указателя на NULL
/// \param [in,out] item - указатель на область памяти
/// \return нет
#define free_null(item)    {if((item) != NULL) free((item)); (item) = NULL;};
#endif

#ifndef do_null
/// Выполнить некое действие, если указатель не равен NULL
/// \param [in] item - указатель на объект или область памяти
/// \param [in] action - блок кода, описывающий действие, например i++
/// \return нет
#define do_null(item, action)    {if((item) != NULL) {action;};}
#endif

#ifndef call_null
/// Вызвать некую функцию объекта, если указатель не равен NULL
/// \param [in] item - указатель на объект
/// \param [in] func - описание вызова функции, например f(x)
/// \return нет
#define call_null(item, func)    {if((item) != NULL) {(item)->func;};}
#endif

#ifndef get_null
/// Получить числовое свойство объекта, если указатель не равен NULL
/// \param [in] item - указатель на объект
/// \param [in] val - свойство объекта или описание вызова функции, например f(x)
/// \return значение свойства или возвращаемое значение функции, если указатель не равен NULL
/// \return 0, если указатель равен NULL
#define get_null(item, val)    ((item) != NULL ? (item)->val : 0)
#endif

#ifndef get_err_null
/// Получить числовое свойство объекта или признак ошибки, если указатель не равен NULL
/// \param [in] item - указатель на объект
/// \param [in] val - свойство объекта или описание вызова функции, например f(x)
/// \return значение свойства или возвращаемое значение функции, если указатель не равен NULL
/// \return ENOMEM (12), если указатель равен NULL
#define get_err_null(item, val)    ((item) != NULL ? (item)->val : ENOMEM)
#endif

#ifndef get_str_null
/// Получить строковое свойство объекта, если указатель не равен NULL
/// \param [in] item - указатель на объект
/// \param [in] str - свойство объекта или описание вызова функции, например f(x)
/// \return значение свойства или возвращаемое значение функции, если указатель не равен NULL
/// \return пустая строка (""), если указатель равен NULL
#define get_str_null(item, str)    ((item) != NULL ? (item)->str : "")
#endif

#ifndef check_null
/// Проверить некое условие, если указатель не равен NULL
/// \param [in] item - указатель на объект
/// \param [in] cond - блок кода, описывающий условие, например handle > -1
/// \return результат проверки условия, если указатель не равен NULL
/// \return FALSE (0), если указатель равен NULL
#define check_null(item, cond)    (item != NULL ? (cond) : FALSE)
#endif

/// @}


/// \defgroup misc_macro Дополнительные макросы
/// @{

/// Реализация foreach для простых массивов
/// \param [in] type - тип массива
/// \param [in, out] item - элемент массива, используемый в цикле
/// \param [in] arr - массив
/// \param [in] arrsiz - размер массива
/// \param [in] iter - итератор массива, объявляется до вызова макроса
#define for_each(type, item, arr, arrsiz, iter) (iter) = 0; for( type item=((iter) < (arrsiz) ? arr[(iter)] : 0) ; (iter) < (arrsiz) ; ++(iter), item=((iter) < (arrsiz) ? arr[(iter)] : 0))

/// Преобразование макроса в текстовую строку
/// \param [in] s макрос
#define STR_MACRO(s) VAL_MACRO(s)

/// Подстановка значения предопределенного макроса
/// \param [in] s макрос
#define VAL_MACRO(s) #s



/// @}


#pragma pack(pop)

#endif