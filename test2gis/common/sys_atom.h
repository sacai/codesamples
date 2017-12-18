#ifndef __SYS_ATOMIC__
#define __SYS_ATOMIC__

/** \file
 	\brief Реализация атомарных операций
 	
Реализация атомарных операций с использованием стандартов языка, либо
платформенно- и компиляторозависимых методов. В данный момент используются функции
стандарта C++11.

Для того, чтобы использовать атомарные типы в обработчиках прерываний QNX, модифицирован
стандартный шаблон простого (POD) атомарного типа таким образом, чтобы промежуточные
переменные в функциях были объявлены как volatile. На базе данного шаблона определены
следующие типы:
 - atomic_int8_t;
 - atomic_int16_t;
 - atomic_int32_t;
 - atomic_int64_t;
 - atomic_uint8_t;
 - atomic_uint16_t;
 - atomic_uint32_t;
 - atomic_uint64_t;
 - atomic_byte_t;
 - atomic_ptrdiff_t;

Для объявления собственного типа необходимо использовать макрос DeclAtomic(), например

\code
typedef DeclAtomic(int)		atomic_int_t;
\endcode

Для операций над атомарными типами определены шаблоны функций
 - AtomicInit() для инициализации значения
 - AtomicGet() для считывания
 - AtomicSet() для присвоения с блокировкой до успеха
 - AtomicTrySet() для однократной попытки присвоения
 - AtomicLockSet() для присвоения до указанного количества попыток
 - AtomicSub() для вычитания
 - AtomicSubFetch() для вычитания с возвратом нового значения
 - AtomicAdd() для сложения
 - AtomicAddFetch() для сложения с возвратом нового значения
 - AtomicCompareAndExchange() для сравнения с обменом
 - AtomicLockCAS() для сравнения с обменом до указанного количества попыток
 - AtomicSwitch() для переключения целочисленного типа из одного состояния в другое
   (например, из 0 в 1)
 - AtomicLockSwitch() для переключения целочисленного типа из одного состояния в другое
   до указанного количества попыток

 */

#pragma pack(push,1)

// Подключим стандартный заголовочный файл
#include <atomic>
#include "sys_defs.h"

#if __GNUC__

/// Реализация шаблона атомарного типа, аналогичная стандартной, с подменой функций,
/// использующих промежуточные переменные на стеке
template<typename _Tp> struct rt_atomic
{
private:
  _Tp _M_i;

public:
  rt_atomic() noexcept = default;
  ~rt_atomic() noexcept = default;
  rt_atomic(const rt_atomic&) = delete;
  rt_atomic& operator=(const rt_atomic&) = delete;
  rt_atomic& operator=(const rt_atomic&) volatile = delete;

  constexpr rt_atomic(_Tp __i) noexcept : _M_i(__i) { }

  operator _Tp() const noexcept
  { return load(); }

  operator _Tp() const volatile noexcept
  { return load(); }

  _Tp
  operator=(_Tp __i) noexcept 
  { store(__i); return __i; }

  _Tp
  operator=(_Tp __i) volatile noexcept 
  { store(__i); return __i; }

  bool
  is_lock_free() const noexcept
  { return __atomic_is_lock_free(sizeof(_M_i), nullptr); }

  bool
  is_lock_free() const volatile noexcept
  { return __atomic_is_lock_free(sizeof(_M_i), nullptr); }

  void
  store(_Tp __i, std::memory_order _m = std::memory_order_seq_cst) noexcept
  { __atomic_store(&_M_i, &__i, _m); }

  void
  store(_Tp __i, std::memory_order _m = std::memory_order_seq_cst) volatile noexcept
  { __atomic_store(&_M_i, &__i, _m); }

  _Tp
  load(std::memory_order _m = std::memory_order_seq_cst) const noexcept
  { 
    volatile _Tp tmp;
    __atomic_load(&_M_i, &tmp, _m); 
    return tmp;
  }

  _Tp
  load(std::memory_order _m = std::memory_order_seq_cst) const volatile noexcept
  { 
    volatile _Tp tmp;
    __atomic_load(&_M_i, &tmp, _m); 
    return tmp;
  }

  _Tp
  exchange(_Tp __i, std::memory_order _m = std::memory_order_seq_cst) noexcept
  { 
    volatile _Tp tmp;
    __atomic_exchange(&_M_i, &__i, &tmp, _m); 
    return tmp;
  }

  _Tp
  exchange(_Tp __i, 
       std::memory_order _m = std::memory_order_seq_cst) volatile noexcept
  { 
    volatile _Tp tmp;
    __atomic_exchange(&_M_i, &__i, &tmp, _m); 
    return tmp;
  }

  bool
  compare_exchange_weak(_Tp& __e, _Tp __i, std::memory_order __s, 
		    std::memory_order __f) noexcept
  {
   return __atomic_compare_exchange(&_M_i, &__e, &__i, true, __s, __f); 
  }

  bool
  compare_exchange_weak(_Tp& __e, _Tp __i, std::memory_order __s, 
		    std::memory_order __f) volatile noexcept
  {
   return __atomic_compare_exchange(&_M_i, &__e, &__i, true, __s, __f); 
  }

  bool
  compare_exchange_weak(_Tp& __e, _Tp __i,
		    std::memory_order __m = std::memory_order_seq_cst) noexcept
  { return compare_exchange_weak(__e, __i, __m,
                                 __cmpexch_failure_order(__m)); }

  bool
  compare_exchange_weak(_Tp& __e, _Tp __i,
	     std::memory_order __m = std::memory_order_seq_cst) volatile noexcept
  { return compare_exchange_weak(__e, __i, __m,
                                 __cmpexch_failure_order(__m)); }

  bool
  compare_exchange_strong(_Tp& __e, _Tp __i, std::memory_order __s, 
		      std::memory_order __f) noexcept
  {
   return __atomic_compare_exchange(&_M_i, &__e, &__i, false, __s, __f); 
  }

  bool
  compare_exchange_strong(_Tp& __e, _Tp __i, std::memory_order __s, 
		      std::memory_order __f) volatile noexcept
  {
    return __atomic_compare_exchange(&_M_i, &__e, &__i, false, __s, __f); 
  }

  bool
  compare_exchange_strong(_Tp& __e, _Tp __i,
		       std::memory_order __m = std::memory_order_seq_cst) noexcept
  {
   return compare_exchange_strong(__e, __i, __m,
                                   __cmpexch_failure_order(__m));
   }

  bool
  compare_exchange_strong(_Tp& __e, _Tp __i,
	     std::memory_order __m = std::memory_order_seq_cst) volatile noexcept
  {
   return compare_exchange_strong(__e, __i, __m,
                                   __cmpexch_failure_order(__m));
  }

  _Tp fetch_add(_Tp __i,
	std::memory_order __m = std::memory_order_seq_cst) noexcept
  { return __atomic_fetch_add(&_M_i, __i, __m); }

  _Tp fetch_add(_Tp  __i,
	std::memory_order __m = std::memory_order_seq_cst) volatile noexcept
  { return __atomic_fetch_add(&_M_i, __i, __m); }

  _Tp fetch_sub(_Tp  __i,
	std::memory_order __m = std::memory_order_seq_cst) noexcept
  { return __atomic_fetch_sub(&_M_i, __i, __m); }

  _Tp fetch_sub(_Tp  __i,
	std::memory_order __m = std::memory_order_seq_cst) volatile noexcept
  { return __atomic_fetch_sub(&_M_i, __i, __m); }

};

/// Макрос для объявления типа
#define DeclAtomic(type) rt_atomic<type>

#else

/// Макрос для объявления типа
#define DeclAtomic(type) std::atomic<type>

#endif

/// \defgroup atomic_types Атомарные типы в дополнение к стандартным
/// @{

typedef DeclAtomic(int8_t)		atomic_int8_t;
typedef DeclAtomic(int16_t)		atomic_int16_t;
typedef DeclAtomic(int32_t)		atomic_int32_t;
typedef DeclAtomic(int64_t)		atomic_int64_t;
typedef DeclAtomic(uint8_t)		atomic_uint8_t;
typedef DeclAtomic(uint16_t)	atomic_uint16_t;
typedef DeclAtomic(uint32_t)	atomic_uint32_t;
typedef DeclAtomic(uint64_t)	atomic_uint64_t;

typedef DeclAtomic(byte_t)			atomic_byte_t;

typedef DeclAtomic(ptrdiff_t)		atomic_ptrdiff_t;

/// @}


/// Инициализация переменной
/// \param [in] loc - указатель на значение атомарной переменной
/// \param [in] value - значение, которое должно быть присвоено данной переменной
/// \return EOK в случае успеха, EINVAL, если указатель не инициализирован
template <class atomT, class T>
int AtomicInit(atomT *loc, T value)
{
	volatile int result = EINVAL;
	if(loc != NULL)
	{
		loc->store(value);
		result = EOK;
	};
	return(result);
};

/// Чтение переменной
/// \param [in] loc - указатель на значение атомарной переменной
/// \return значение переменной, полученное по данному указателю,
/// \return NEGATIVE в случае ошибки
#define AtomicGet(loc) ((loc) != NULL ? (loc)->load() : NEGATIVE)

/// Блокируемое присвоение переменной
/// \param [in, out] loc - указатель на значение атомарной переменной
/// \param [in] value - значение, которое должно быть присвоено данной переменной
/// \param [in] tries - количество попыток изменить значение переменной
/// (по умолчанию 0, до успеха)
/// \return EOK в случае успеха,
/// \return EINVAL в случае неверных параметров
/// \return EBUSY, если не удалось присвоить значение
template <class atomT, class T>
int AtomicSet(atomT *loc, T value, int tries = 0)
{
	volatile int result = EINVAL;
	if(loc != NULL)
	{
		result = EOK;
		volatile T oldvar;
		volatile int i = 0;
		volatile int Tries = tries;
		if(Tries < 0) Tries = 1;
		while((Tries ? i < Tries : true))
		{
			oldvar = loc->load();
			if(loc->exchange(value) == oldvar)
				return(result);
			if(Tries) i++;
		};
		result = EBUSY;
	};
	return(result);
};

/// Присвоение переменной (однократная попытка)
/// \param [in, out] loc - указатель на значение атомарной переменной
/// \param [in] value - значение, которое должно быть присвоено данной переменной
/// \return EOK в случае успеха,
/// \return EINVAL в случае неверных параметров
/// \return EBUSY, если не удалось присвоить значение
template <class atomT, class T>
int AtomicTrySet(atomT *loc, T value)
{
	return(AtomicSet(loc, value, -1));
};

/// Присвоение переменной с блокировкой до указанного числа попыток
/// \param [in, out] loc - указатель на значение атомарной переменной
/// \param [in] value - значение, которое должно быть присвоено данной переменной
/// \param [in] tries - максимальное количество попыток изменения переменной
/// \return EOK в случае успеха,
/// \return EINVAL в случае неверных параметров
/// \return EBUSY, если не удалось присвоить значение
template <class atomT, typename T>
int AtomicLockSet(atomT *loc, T value, int tries)
{
	return(AtomicSet(loc, value, tries));
};

/// Вычитание переменной
/// \param [in, out] loc - указатель на значение атомарной переменной
/// \param [in] value - значение, которое должно быть вычтено из данной переменной
/// \return предшествующее значение переменной
/// \return NEGATIVE в случае ошибки
template <class atomT, class T>
T AtomicSub(atomT *loc, T value)
{
	return((loc != NULL ? loc->fetch_sub(value) : (T)NEGATIVE));
};

/// Вычитание переменной с возвратом нового значения
/// \param [in, out] loc - указатель на значение атомарной переменной
/// \param [in] value - значение, которое должно быть вычтено из данной переменной
/// \return новое значение переменной
/// \return NEGATIVE в случае ошибки
template <class atomT, class T>
T AtomicSubFetch(atomT *loc, T value)
{
	if(loc != NULL)
	{
		loc->fetch_sub(value);
		return(loc->load());
	}
	return((T)NEGATIVE);
};

/// Сложение переменной
/// \param [in, out] loc - указатель на значение атомарной переменной
/// \param [in] value - значение, которое должно быть вычтено из данной переменной
/// \return предшествующее значение переменной
/// \return NEGATIVE в случае ошибки
template <class atomT, class T>
T AtomicAdd(atomT *loc, T value)
{
	return((loc != NULL ? loc->fetch_add(value) : (T)NEGATIVE));
};

/// Сложение переменной с возвратом нового значения
/// \param [in, out] loc - указатель на значение атомарной переменной
/// \param [in] value - значение, которое должно быть вычтено из данной переменной
/// \return новое значение переменной
/// \return NEGATIVE в случае ошибки
template <class atomT, class T>
T AtomicAddFetch(atomT *loc, T value)
{
	if(loc != NULL)
	{
		loc->fetch_add(value);
		return(loc->load());
	}
	return((T)NEGATIVE);
};

/// Неблокируемое сравнение и изменение значения переменной
/// \param [in, out] loc - указатель на значение атомарной переменной, которое нужно изменить
/// \param [in, out] from - указатель на буфер с ожидаемым значением переменной.
/// Если обмен не удастся произвести, в буфер будет помещено текущее значение переменной
/// \param [in] to - значение, которое нужно поместить в переменную при условии равенства текущего и ожидаемого значений
/// \return true при успешном сравнении и обмене, false в противном случае
template <class atomT, class T>
bool AtomicCompareAndExchange(atomT *loc, T &from, T to)
{
	return(((loc != NULL) ? loc->compare_exchange_strong(from, to) : false));
};

template <class atomT, class T>
bool AtomicCompareAndExchange(atomT *loc, volatile T &from, volatile T to)
{
	return(((loc != NULL) ? loc->compare_exchange_strong((T&)from, (T)to) : false));
};

/// Сравнение и изменение значения переменной с блокировкой на заданное количества попыток
/// \param [in, out] loc - указатель на значение атомарной переменной, которое нужно изменить
/// \param [in, out] from - указатель на буфер с ожидаемым значением переменной
/// \param [in] to - значение, которое нужно поместить в переменную при условии равенства текущего и ожидаемого значений
/// \param [in] tries - максимальное количество попыток (0 - бесконечная блокировка до успеха)
/// \return EOK в случае успеха,
/// \return EINVAL в случае неверных параметров,
/// \return EBUSY, если не удалось присвоить значение за указанное число попыток
template <class atomT, class T>
int AtomicLockCAS(atomT *loc, T &from, T to, int tries = 0)
{
	volatile int result = EINVAL;
	if(loc != NULL && from != NULL)
	{
		volatile int i = 0;
		volatile int Tries = tries;
		volatile T desired = from;
		result = EOK;
		if(Tries < 0) Tries = 1;
		while((Tries ? i < Tries : true))
		{
			from = desired;
			if(loc->load() == desired && AtomicCompareAndExchange(loc, from, to))
				return(result);
			if(Tries) i++;
		}
		result = EBUSY;
	}
	return(result);
};

/// Переключение значения переменной из одного состояния в другое
/// \param [in, out] loc - указатель на значение атомарной переменной, которое нужно изменить
/// \param [in] from - исходное значение переменной
/// \param [in] to - значение, в которое нужно переключить переменную
/// \return EOK в случае успеха,
/// \return EINVAL в случае неверных параметров
template <class atomT, class T>
int AtomicSwitch(atomT *loc, T from, T to)
{
	volatile T __from = from;
	if(loc != NULL && AtomicCompareAndExchange(loc, (T&)__from, to))
		return(EOK);
	return(EINVAL);
};

/// Переключение значения переменной из одного состояния в другое
/// с блокировкой на заданное количества попыток
/// \param [in, out] loc - указатель на значение атомарной переменной, которое нужно изменить
/// \param [in] from - исходное значение переменной
/// \param [in] to - значение, в которое нужно переключить переменную
/// \param [in] tries - максимальное количество попыток (0 - бесконечная блокировка до успеха)
/// \return EOK в случае успеха,
/// \return EINVAL в случае неверных параметров,
/// \return EBUSY, если не удалось присвоить значение за указанное число попыток
template <class atomT, class T>
int AtomicLockSwitch(atomT *loc, T from, T to, int tries = 0)
{
	volatile int result = EINVAL;
	if(loc != NULL)
	{
		volatile int i = 0;
		volatile int Tries = tries;
		volatile T __from = from;
		result = EOK;
		if(Tries < 0) Tries = 1;
		while((Tries ? i < Tries : true))
		{
			__from = from;
  			if(AtomicCompareAndExchange(loc, (T&)__from, to))
				return(result);
			if(Tries) i++;
		}
		result = EBUSY;
	}
	return(result);
};

#pragma pack(pop)

#endif