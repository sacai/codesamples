#ifndef __SYS_ATOMIC__
#define __SYS_ATOMIC__

/** \file
 	\brief ��������� �⮬���� ����権
 	
��������� �⮬���� ����権 � �ᯮ�짮������ �⠭���⮢ �몠, ����
�����ଥ���- � ��������஧���ᨬ�� ��⮤��. � ����� ������ �ᯮ������� �㭪樨
�⠭���� C++11.

��� ⮣�, �⮡� �ᯮ�짮���� �⮬��� ⨯� � ��ࠡ��稪�� ���뢠��� QNX, ������஢��
�⠭����� 蠡��� ���⮣� (POD) �⮬�୮�� ⨯� ⠪�� ��ࠧ��, �⮡� �஬������
��६���� � �㭪��� �뫨 ������ ��� volatile. �� ���� ������� 蠡���� ��।�����
᫥���騥 ⨯�:
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

��� ������� ᮡ�⢥����� ⨯� ����室��� �ᯮ�짮���� ����� DeclAtomic(), ���ਬ��

\code
typedef DeclAtomic(int)		atomic_int_t;
\endcode

��� ����権 ��� �⮬��묨 ⨯��� ��।����� 蠡���� �㭪権
 - AtomicInit() ��� ���樠����樨 ���祭��
 - AtomicGet() ��� ���뢠���
 - AtomicSet() ��� ��᢮���� � �����஢��� �� �ᯥ�
 - AtomicTrySet() ��� ������⭮� ����⪨ ��᢮����
 - AtomicLockSet() ��� ��᢮���� �� 㪠������� ������⢠ ����⮪
 - AtomicSub() ��� ���⠭��
 - AtomicSubFetch() ��� ���⠭�� � �����⮬ ������ ���祭��
 - AtomicAdd() ��� ᫮�����
 - AtomicAddFetch() ��� ᫮����� � �����⮬ ������ ���祭��
 - AtomicCompareAndExchange() ��� �ࠢ����� � �������
 - AtomicLockCAS() ��� �ࠢ����� � ������� �� 㪠������� ������⢠ ����⮪
 - AtomicSwitch() ��� ��४��祭�� 楫��᫥����� ⨯� �� ������ ���ﭨ� � ��㣮�
   (���ਬ��, �� 0 � 1)
 - AtomicLockSwitch() ��� ��४��祭�� 楫��᫥����� ⨯� �� ������ ���ﭨ� � ��㣮�
   �� 㪠������� ������⢠ ����⮪

 */

#pragma pack(push,1)

// ������稬 �⠭����� ���������� 䠩�
#include <atomic>
#include "sys_defs.h"

#if __GNUC__

/// ��������� 蠡���� �⮬�୮�� ⨯�, �������筠� �⠭���⭮�, � �������� �㭪権,
/// �ᯮ������ �஬������ ��६���� �� �⥪�
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

/// ����� ��� ������� ⨯�
#define DeclAtomic(type) rt_atomic<type>

#else

/// ����� ��� ������� ⨯�
#define DeclAtomic(type) std::atomic<type>

#endif

/// \defgroup atomic_types �⮬��� ⨯� � ���������� � �⠭�����
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


/// ���樠������ ��६�����
/// \param [in] loc - 㪠��⥫� �� ���祭�� �⮬�୮� ��६�����
/// \param [in] value - ���祭��, ���஥ ������ ���� ��᢮��� ������ ��६�����
/// \return EOK � ��砥 �ᯥ�, EINVAL, �᫨ 㪠��⥫� �� ���樠����஢��
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

/// �⥭�� ��६�����
/// \param [in] loc - 㪠��⥫� �� ���祭�� �⮬�୮� ��६�����
/// \return ���祭�� ��६�����, ����祭��� �� ������� 㪠��⥫�,
/// \return NEGATIVE � ��砥 �訡��
#define AtomicGet(loc) ((loc) != NULL ? (loc)->load() : NEGATIVE)

/// ������㥬�� ��᢮���� ��६�����
/// \param [in, out] loc - 㪠��⥫� �� ���祭�� �⮬�୮� ��६�����
/// \param [in] value - ���祭��, ���஥ ������ ���� ��᢮��� ������ ��६�����
/// \param [in] tries - ������⢮ ����⮪ �������� ���祭�� ��६�����
/// (�� 㬮�砭�� 0, �� �ᯥ�)
/// \return EOK � ��砥 �ᯥ�,
/// \return EINVAL � ��砥 ������� ��ࠬ��஢
/// \return EBUSY, �᫨ �� 㤠���� ��᢮��� ���祭��
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

/// ��᢮���� ��६����� (������⭠� ����⪠)
/// \param [in, out] loc - 㪠��⥫� �� ���祭�� �⮬�୮� ��६�����
/// \param [in] value - ���祭��, ���஥ ������ ���� ��᢮��� ������ ��६�����
/// \return EOK � ��砥 �ᯥ�,
/// \return EINVAL � ��砥 ������� ��ࠬ��஢
/// \return EBUSY, �᫨ �� 㤠���� ��᢮��� ���祭��
template <class atomT, class T>
int AtomicTrySet(atomT *loc, T value)
{
	return(AtomicSet(loc, value, -1));
};

/// ��᢮���� ��६����� � �����஢��� �� 㪠������� �᫠ ����⮪
/// \param [in, out] loc - 㪠��⥫� �� ���祭�� �⮬�୮� ��६�����
/// \param [in] value - ���祭��, ���஥ ������ ���� ��᢮��� ������ ��६�����
/// \param [in] tries - ���ᨬ��쭮� ������⢮ ����⮪ ��������� ��६�����
/// \return EOK � ��砥 �ᯥ�,
/// \return EINVAL � ��砥 ������� ��ࠬ��஢
/// \return EBUSY, �᫨ �� 㤠���� ��᢮��� ���祭��
template <class atomT, typename T>
int AtomicLockSet(atomT *loc, T value, int tries)
{
	return(AtomicSet(loc, value, tries));
};

/// ���⠭�� ��६�����
/// \param [in, out] loc - 㪠��⥫� �� ���祭�� �⮬�୮� ��६�����
/// \param [in] value - ���祭��, ���஥ ������ ���� ���⥭� �� ������ ��६�����
/// \return �।�����饥 ���祭�� ��६�����
/// \return NEGATIVE � ��砥 �訡��
template <class atomT, class T>
T AtomicSub(atomT *loc, T value)
{
	return((loc != NULL ? loc->fetch_sub(value) : (T)NEGATIVE));
};

/// ���⠭�� ��६����� � �����⮬ ������ ���祭��
/// \param [in, out] loc - 㪠��⥫� �� ���祭�� �⮬�୮� ��६�����
/// \param [in] value - ���祭��, ���஥ ������ ���� ���⥭� �� ������ ��६�����
/// \return ����� ���祭�� ��६�����
/// \return NEGATIVE � ��砥 �訡��
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

/// �������� ��६�����
/// \param [in, out] loc - 㪠��⥫� �� ���祭�� �⮬�୮� ��६�����
/// \param [in] value - ���祭��, ���஥ ������ ���� ���⥭� �� ������ ��६�����
/// \return �।�����饥 ���祭�� ��६�����
/// \return NEGATIVE � ��砥 �訡��
template <class atomT, class T>
T AtomicAdd(atomT *loc, T value)
{
	return((loc != NULL ? loc->fetch_add(value) : (T)NEGATIVE));
};

/// �������� ��६����� � �����⮬ ������ ���祭��
/// \param [in, out] loc - 㪠��⥫� �� ���祭�� �⮬�୮� ��६�����
/// \param [in] value - ���祭��, ���஥ ������ ���� ���⥭� �� ������ ��६�����
/// \return ����� ���祭�� ��६�����
/// \return NEGATIVE � ��砥 �訡��
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

/// ��������㥬�� �ࠢ����� � ��������� ���祭�� ��६�����
/// \param [in, out] loc - 㪠��⥫� �� ���祭�� �⮬�୮� ��६�����, ���஥ �㦭� ��������
/// \param [in, out] from - 㪠��⥫� �� ���� � �������� ���祭��� ��६�����.
/// �᫨ ����� �� 㤠���� �ந�����, � ���� �㤥� ����饭� ⥪�饥 ���祭�� ��६�����
/// \param [in] to - ���祭��, ���஥ �㦭� �������� � ��६����� �� �᫮��� ࠢ���⢠ ⥪�饣� � ���������� ���祭��
/// \return true �� �ᯥ譮� �ࠢ����� � ������, false � ��⨢��� ��砥
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

/// �ࠢ����� � ��������� ���祭�� ��६����� � �����஢��� �� �������� ������⢠ ����⮪
/// \param [in, out] loc - 㪠��⥫� �� ���祭�� �⮬�୮� ��६�����, ���஥ �㦭� ��������
/// \param [in, out] from - 㪠��⥫� �� ���� � �������� ���祭��� ��६�����
/// \param [in] to - ���祭��, ���஥ �㦭� �������� � ��६����� �� �᫮��� ࠢ���⢠ ⥪�饣� � ���������� ���祭��
/// \param [in] tries - ���ᨬ��쭮� ������⢮ ����⮪ (0 - ��᪮��筠� �����஢�� �� �ᯥ�)
/// \return EOK � ��砥 �ᯥ�,
/// \return EINVAL � ��砥 ������� ��ࠬ��஢,
/// \return EBUSY, �᫨ �� 㤠���� ��᢮��� ���祭�� �� 㪠������ �᫮ ����⮪
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

/// ��४��祭�� ���祭�� ��६����� �� ������ ���ﭨ� � ��㣮�
/// \param [in, out] loc - 㪠��⥫� �� ���祭�� �⮬�୮� ��६�����, ���஥ �㦭� ��������
/// \param [in] from - ��室��� ���祭�� ��६�����
/// \param [in] to - ���祭��, � ���஥ �㦭� ��४����� ��६�����
/// \return EOK � ��砥 �ᯥ�,
/// \return EINVAL � ��砥 ������� ��ࠬ��஢
template <class atomT, class T>
int AtomicSwitch(atomT *loc, T from, T to)
{
	volatile T __from = from;
	if(loc != NULL && AtomicCompareAndExchange(loc, (T&)__from, to))
		return(EOK);
	return(EINVAL);
};

/// ��४��祭�� ���祭�� ��६����� �� ������ ���ﭨ� � ��㣮�
/// � �����஢��� �� �������� ������⢠ ����⮪
/// \param [in, out] loc - 㪠��⥫� �� ���祭�� �⮬�୮� ��६�����, ���஥ �㦭� ��������
/// \param [in] from - ��室��� ���祭�� ��६�����
/// \param [in] to - ���祭��, � ���஥ �㦭� ��४����� ��६�����
/// \param [in] tries - ���ᨬ��쭮� ������⢮ ����⮪ (0 - ��᪮��筠� �����஢�� �� �ᯥ�)
/// \return EOK � ��砥 �ᯥ�,
/// \return EINVAL � ��砥 ������� ��ࠬ��஢,
/// \return EBUSY, �᫨ �� 㤠���� ��᢮��� ���祭�� �� 㪠������ �᫮ ����⮪
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