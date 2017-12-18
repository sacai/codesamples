#ifndef __SYS_BASE_CLASS__
#define __SYS_BASE_CLASS__

/** \file
 \brief Базовый класс для всех объектов системы

	Базовый класс для всез объектов системы. Хранит признак ошибки, доступный в производном классе через функции

*/

#include <errno.h>
#include "sys_defs.h"

#pragma pack(push,1)

extern "C" {

/// Базовыый класс для всех объектов системы
class DRTS_Base_Object
{
 volatile uint32_t ei;          ///< Текущий признак ошибки
public:

 /// Установить признак ошибки
 /// \param [in] uint32_t err значение признака ошибки
 /// \return нет
 void setError(uint32_t err) {ei=err;};

 /// Сбросить признак ошибки
 /// \param [in] uint32_t err значение признака ошибки
 /// \return нет
 void clearError(uint32_t err) {ei=EOK;};

 /// Прочитать признак ошибки
 /// \param нет
 /// \return значение признака ошибки
 uint32_t getError() {return ei;};

 /// Конструктор 
 /// \param нет
 DRTS_Base_Object() {ei=EOK;};

 /// Деструктор
 /// \param нет
 ~DRTS_Base_Object() {ei=EOK;};

};

};

#pragma pack(pop)

#endif