#ifndef __DRTS_ARRAY_T__
#define __DRTS_ARRAY_T__

/** \file
	\brief ������ ����ࠪ⭮�� �������᪮�� ���ᨢ�

	������ ����ࠪ⭮�� �������᪮�� ���ᨢ�. �ᯮ������ ��� �᭮�� ��� ���ᨢ�,
	�।�����祭���� ��� �࠭���� ����⨯��� ����⮢ � ������������ �� ����������,
	��⠢�� � ��।������� ������ � 㤠�����. �������᭠� ������ �⠭���⭮��
	������, ��࠭������, �� ������ �� �����।������ �� ����㯥 � ����⠬
	���ᨢ� �, ⠪�� ��ࠧ��, ����᪠��� �ᯮ�짮����� � ��ࠡ��稪�� ᨣ����� �
	���뢠���.
	
	����� ���� ⠪�� �ᯮ�짮��� � ����⢥ �������� ����� �� ᮧ����� ���ᨢ��
	� ᯥ���묨 �㭪�ﬨ.

*/

#pragma pack(push,1)

#include <stdint.h>
#include "sys_base.h"
#include "sys_mem.h"
#include "sys_atom.h"

/// ������ ����ࠪ⭮�� �������᪮�� ���ᨢ�
template <class T>
class array_t : public DRTS_Base_Object
{
	T *arr;						///< ���ᨢ ����⮢ ��������� ⨯�
	size_t a_delta;				///< ���饭��
	size_t a_nfree;				///< ���-�� ᢮������ ����⮢
	size_t a_size;				///< ���-�� �ᯮ��㥬�� ����⮢
	size_t a_capacity;			///< ���ᨬ��쭮� ������⢮ ����⮢ � ���ᨢ�
	atomic_uint32_t in_use;		///< �ਧ��� �����஢�� ���ᨢ�
	T invalid_value;			///< �������⨬�� ���祭�� �����
	
public:
	/// ��������� �� 㬮�砭��
	array_t()
	{
		init();
	};

	/// ���������
	/// \param [in] size_t startsize ��室�� ࠧ��� ���ᨢ�
	/// \param [in] size_t delta ������⢮ ����⮢, �� ���஥ 㢥��稢����� ࠧ���
	/// ���ᨢ� � �����।������ ������ �� ���௠��� ᢮������� ���� (�� 㬮�砭��
	/// ��ࠢ�������� �������� ��室���� ࠧ���)
	array_t(size_t startsize, size_t delta = 0)
	{
		init();
		init(startsize, delta);
	};

	/// ��������� ����஢����
	/// \param [in] const array_t<T> &src ��室�� ���ᨢ
	array_t(const array_t<T> &src)
	{
		init();
		if(init(src.a_capacity, src.a_delta) == EOK)
		{
			for(int i = 0; i < src.a_size; i++)
			{
				T item = src.arr[i];
				int result = EOK;
				if(valid(item))
					arr[i] = make_item(item);
				else
					arr[i] = make_empty();
				a_size++;
				a_nfree--;
				if(!valid(arr[i]))
					break;
			}
		}
	};

	/// ��������
	~array_t()
	{
		clear();
	};

	/// ���樠������ ���ᨢ�
	/// \param [in] size_t startsize ��室�� ࠧ��� ���ᨢ�
	/// \param [in] size_t delta ������⢮ ����⮢, �� ���஥ 㢥��稢����� ࠧ���
	/// ���ᨢ� � �����।������ ������ �� ���௠��� ᢮������� ���� (�� 㬮�砭��
	/// ��ࠢ�������� �������� ��室���� ࠧ���)
	/// \return EOK (0) � ��砥 �ᯥ�
	/// \return EINVAL (22) �� ������� ��ࠬ����
	/// \return ENOMEM (12) �� �訡�� �뤥����� �����
	int init(size_t startsize, size_t delta = 0)
	{
		int result = EINVAL;
    
		result = DRTSMallocZero(arr, startsize * sizeof(T));
		if(result == EOK)
		{
			for(int i = 0; i < startsize; i++)
				arr[i] = invalid();
			a_delta = (delta > 0 ? delta : startsize/2);
			a_nfree = startsize;
			a_size = 0;
			a_capacity = startsize;
		}
    
		setError(result);
		return(result);
	};

	/// ���樠������ ���⮣� ���ᨢ�
	/// \param ���
	/// \return EOK (0) � ��砥 �ᯥ�
	int init(void)
	{
		int result = EOK;
    
		arr = NULL;
		a_delta = 0;
		a_nfree = 0;
		a_size = 0;
		a_capacity = 0;
		AtomicInit(&in_use, 0);
		invalid_value = (T)-1;
    
		setError(result);
		return(result);
	};

	/// ���⪠ ���ᨢ� � �᢮��������� �����
	/// \param ���
	/// \return ���
	void clear(void)
	{
		Lock();
		DRTSFree(arr);
		a_delta = 0;
		a_nfree = 0;
		a_size = 0;
		a_capacity = 0;
		UnLock();
	};

	/// ���������� ����� � ���ᨢ
	/// \param [in] T item ������塞� �����
	/// \param [in] bool lck �����஢��� �� ���ᨢ (true) ��� ��� (false),
	/// �� 㬮�砭�� �����஢���
	/// \return EOK (0) � ��砥 �ᯥ�
	/// \return ENOMEM (12) �� �訡�� �뤥����� �����
	int add(T item, bool lck = true)
	{
		int result = EOK;
        
		if(lck)
			result = Lock();
		if(result == EOK)
		{
			if(!a_nfree)
			{
				result = DRTSReallocZero(arr, a_size*sizeof(T), (a_size+a_delta)*sizeof(T));
				if(result == EOK)
				{
					a_capacity = a_size + a_delta;
					a_nfree = a_delta;
				}
			}
			if(result == EOK)
			{
				arr[a_size] = make_item(item);
				a_size++;
				a_nfree--;
			}
			if(lck)
				UnLock();
		}
		setError(result);
		return(result);
	};

	/// ���������� ����� � ���ᨢ � ��� ᮧ������
	/// \param [in] bool lck �����஢��� �� ���ᨢ (true) ��� ��� (false),
	/// �� 㬮�砭�� �����஢���
	/// \return EOK (0) � ��砥 �ᯥ�
	/// \return ENOMEM (12) �� �訡�� �뤥����� �����
	int add(bool lck = true) {return(add(make_empty(), lck));};

	/// �������� ���⮣� �����. �㭪�� ����� ���� ��८�।����� � �ந������� �����.
	/// \param ���
	/// \return �����樠����஢���� ����� ���ᨢ� (0)
	/// \return invalid() � ��砥 �訡��
	/// (�ਧ��� �訡�� ������ ���� �����饭 � getError())
	virtual T make_empty(void) {T item = {0}; return(item);};

	/// �������� �����. �㭪�� ����� ���� ��८�।����� � �ந������� �����.
	/// \param [in] T src ��室��� ���祭�� �����
	/// \return �����樠����஢���� ����� ���ᨢ� (0)
	/// \return invalid() � ��砥 �訡��
	/// (�ਧ��� �訡�� ������ ���� �����饭 � getError())
	virtual T make_item(T src) {T item = src; return(item);};
	
	/// ��⠢�� ����� � ���ᨢ �� �������� ������
	/// \param [in] T item ������塞� �����
	/// \param [in] size_t pos ������
	/// \param [in] bool lck �����஢��� �� ���ᨢ (true) ��� ��� (false),
	/// �� 㬮�砭�� �����஢���
	/// \return EOK (0) � ��砥 �ᯥ�
	/// \return ENOMEM (12) �� �訡�� �뤥����� �����
	/// \return ENOENT (2), �᫨ ������ ��室�� �� �।��� ���ᨢ�
	int insertAt(T item, size_t pos, bool lck = true)
	{
		int result = EOK;
        
		if(lck)
			result = Lock();
		if(result == EOK)
		{
			if(pos >= a_size)
				result = ENOENT;
			else
			{
				if(!a_nfree)
				{
					result = DRTSReallocZero(arr, a_size*sizeof(T), (a_size+a_delta)*sizeof(T));
					if(result == EOK)
					{
						a_capacity = a_size + a_delta;
						a_nfree = a_delta;
					}
				}
				if(result == EOK)
				{
					memmove(&arr[pos+1], &arr[pos], (a_size-pos)*sizeof(T));
					arr[pos]=make_item(item);
					a_size++;
					a_nfree--;
				}
			}
			if(lck)
				UnLock();
		}
		setError(result);
		return(result);
	};

	/// ��᢮���� ���祭�� ����� �� �������� ����樨
	/// \param [in] size_t pos ������
	/// \param [in] T item ���祭�� �����
	/// \param [in] bool lck �����஢��� �� ���ᨢ (true) ��� ��� (false),
	/// �� 㬮�砭�� �����஢���
	/// \return EOK (0) � ��砥 �ᯥ�
	/// \return ENOENT (2), �᫨ ������ ��室�� �� �।��� ���ᨢ�
	int set(T item, size_t pos, bool lck = true)
	{
		int result = EOK;
    
		if(lck)
			result = Lock();
		if(result == EOK)
		{
			if(pos >= a_size)
				result = ENOENT;
			else
				arr[pos] = make_item(item);
			if(lck)
				UnLock();
		}
		setError(result);
		return(result);
	};

	/// ����� � ������ ���ᨢ�
	/// \param [in] size_t index ������
	/// \param [in] bool lck �����஢��� �� ���ᨢ (true) ��� ��� (false),
	/// �� 㬮�砭�� �����஢���
	/// \return invalid() � ��砥 �訡��
	/// (�ਧ��� �訡�� ������ ���� �����饭 � getError())
	T get(size_t index, bool lck = true)
	{
		int result = EOK;
		T item = invalid();
    
		if(lck)
			result = Lock();
		if(result == EOK)
		{
			if(!a_size || index >= a_size)
				result = ENOENT;
			else
				item = arr[index];
			if(lck)
				UnLock();
		}
		setError(result);
		return(item);
	};

	/// ����� � ������ ���ᨢ� (������ [])
	/// \param [in] size_t index ������
	/// \return invalid() � ��砥 �訡��
	/// (�ਧ��� �訡�� ������ ���� �����饭 � getError())
	T operator[](size_t index) {return(get(index));};

	/// ����� � ��ࢮ�� ������ ���ᨢ�
	/// \param ���
	/// \return invalid() � ��砥 �訡��
	/// (�ਧ��� �訡�� ������ ���� �����饭 � getError())
	T getfirst(void)
	{
		if(a_size) return(get(0));
		setError(ENOENT);
		return invalid();
	};

	/// ����� � ��᫥����� ������ ���ᨢ�
	/// \param ���
	/// \return invalid() � ��砥 �訡��
	/// (�ਧ��� �訡�� ������ ���� �����饭 � getError())
	T getlast(void)
	{
		if(a_size) return(get(a_size - 1));
		setError(ENOENT);
		return invalid();
	};

	/// ���⪠ �����. �㭪�� ����� ���� ��८�।����� � �ந������� �����.
	/// \param [in] T &item ��뫪� �� ��頥�� �����
	/// \return EOK (0) � ��砥 �ᯥ�
	/// \return ENOMEM (12) �� �訡�� �᢮�������� �����
	virtual int clean_item(T &item) {item = {0}; setError(EOK); return(EOK);};

	/// �������� 㪠������� �����
	/// \param [in] size_t pos ������
	/// \param [in] bool lck �����஢��� �� ���ᨢ (true) ��� ��� (false),
	/// �� 㬮�砭�� �����஢���
	/// \param [in] bool clean ����� �� ����� (true) ��� ��� (false),
	/// �� 㬮�砭�� �����
	/// \return EOK (0) � ��砥 �ᯥ�
	/// \return ENOENT (2), �᫨ ������ ��室�� �� �।��� ���ᨢ�
	int del(size_t pos, bool lck = true, bool clean = true)
	{
		int result = EOK;
        
		if(lck)
			result = Lock();
		if(result == EOK)
		{
			if(pos >= a_size)
				result = ENOENT;
			else
			{
				if(clean)
					clean_item(arr[pos]);
				memmove(&arr[pos], &arr[pos+1], (a_size-pos-1)*sizeof(T));
				a_size--;
				a_nfree++;
				arr[a_size] = invalid();
			}
			if(lck)
				UnLock();
		}
		setError(result);
		return(result);
	};

	/// �������� ��᫥����� �����
	/// \param [in] bool lck �����஢��� �� ���ᨢ (true) ��� ��� (false),
	/// �� 㬮�砭�� �����஢���
	/// \param [in] bool clean ����� �� ����� (true) ��� ��� (false),
	/// �� 㬮�砭�� �����
	/// \return EOK (0) � ��砥 �ᯥ�
	/// \return ENOENT (2), �᫨ ���ᨢ ����
	int removelast(bool lck = true, bool clean = true)
	{
		int result = EOK;
        
		if(lck)
			result = Lock();
		if(result == EOK)
		{
			if(!a_size)
				result = ENOENT;
			else
			{
				if(clean)
					clean_item(arr[a_size - 1]);
				a_size--;
				a_nfree++;
			}
			if(lck)
				UnLock();
		}
		setError(result);
		return(result);
	};

	/// ���⪠ ���ᨢ� ��� �᢮�������� �����, �뤥������ ��� ���ᨢ
	/// \param [in] bool lck �����஢��� �� ���ᨢ (true) ��� ��� (false),
	/// �� 㬮�砭�� �����஢���
	/// \param [in] bool clean ����� �� ������ (true) ��� ��� (false),
	/// �� 㬮�砭�� �����
	/// \return EOK (0) � ��砥 �ᯥ�
	/// \return ENOENT (2), �᫨ ���ᨢ ����
	int flush(bool lck = true, bool clean = true)
	{
		int result = EOK;
        
		if(lck)
			result = Lock();
		if(result == EOK)
		{
			if(!a_size)
				result = ENOENT;
			else
			{
				if(clean)
				{
					for(int i = 0; i < a_size; i++)
					{
						clean_item(arr[i]);
					}
				}
				a_nfree = a_capacity;
				a_size = 0;
			}
			if(lck)
				UnLock();
		}
		setError(result);
		return(result);
	};

	/// �����஢�� ���ᨢ� �� ���������
    /// \param [in] int tries - ���ᨬ��쭮� ������⢮ ����⮪ (�� 㬮�砭�� 10)
    /// \return EOK (0) � ��砥 �ᯥ�
    /// \return EBUSY (16) �� ������������ �����஢��
	int Lock(int tries = 10)
	{
		int result = AtomicLockSwitch(&in_use, 0U, 1U, tries);
		setError(result);
		return(result);
	};

	/// ����祭�� �ਧ���� �����஢�� ���ᨢ�
    /// \param ���
    /// \return ���祭�� �ਧ���� �����஢�� (1 ��� 0)
	uint32_t GetUsed(void) {return(AtomicGet(&in_use));};

	/// ���⨥ �����஢�� ���ᨢ�
    /// \param [in] int tries - ���ᨬ��쭮� ������⢮ ����⮪ (�� 㬮�砭�� 10)
    /// \return EOK (0) � ��砥 �ᯥ�
    /// \return EBUSY (16) �� ������������ �����஢��
    /// \return EINVAL (22), �᫨ ���ᨢ �� �������஢��
	int UnLock(int tries = 10)
	{
		int result = EINVAL;
		if(GetUsed())
			result = AtomicLockSwitch(&in_use, 1U, 0U, tries);
		setError(result);
		return(result);
	};

	/// ������� ⥪�騩 ࠧ��� ���ᨢ�
	/// \param ���
	/// \return ࠧ��� ���ᨢ�
	size_t size() {return(a_size);};

	/// ������� ���ᨬ���� ࠧ��� ���ᨢ�
	/// \param ���
	/// \return ���ᨬ���� ࠧ��� ���ᨢ�
	size_t capacity() {return(a_capacity);};

	/// ������� �����।�⢥��� ����� � ���ᨢ� (���ਬ��, ��� ���஢��)
	/// \param ���
	/// \return 㪠��⥫� �� ���ᨢ ��� NULL, �᫨ ���ᨢ �� ���樠����஢��
	/// (��᫥ �맮�� ��������� �� 㬮�砭�� ��� clear())
	const T* c_array() {return(arr);};

	/// ��।�����, ���� �� �����⨬� ����� ���ᨢ�
	/// \param [in] item �஢��塞�� ���祭�� �����
	/// \return true, �᫨ ����� ���� �����⨬�
	/// \return false, �᫨ �� ����
	bool valid(T item) {return(item != invalid());};

	/// �����頥� �������⨬�� ���祭�� ����� ���ᨢ� (�� 㬮�砭�� -1).
	/// ����� ���� ��८�।����� � �ந������� �����, ���ਬ��. �᫨
	/// �������⨬� ���祭��� ���� ���� ��㣮�
	/// \param ���
	/// \return �������⨬�� ���祭�� �����
	T invalid(void) {return(invalid_value);};

	/// ��⠭����� �������⨬�� ���祭�� ����� ���ᨢ�
	/// \param [in] value �������⨬�� ���祭�� �����
	/// \return ���
	void set_invalid(T value) {invalid_value = value;};

};

/// ��������� foreach ��� �������᪨� ���ᨢ�� �� ���� array_t.
/// ����஫� �� �ࠢ��쭮��� ����� ����������� �� �ணࠬ����.
/// \param [in] type - ⨯ ����� � ���ᨢ�
/// \param [in, out] item - ����� ���ᨢ�, �ᯮ��㥬� � 横��
/// \param [in] arr - ��ꥪ�-���ᨢ
/// \param [in] iter - ����� ���ᨢ�, ������� �� �맮�� �����
#define for_each_array(type, item, arr, iter) (iter) = 0; for( type item=(type)(arr).get(iter) ; iter < (arr).size() ; ++iter, item=(type)(arr).get(iter))

/// ��������� foreach ��� �������᪨� ���ᨢ�� �� ���� array_t
/// � ����஫�� �� �ࠢ��쭮��� �����.
/// \param [in] type - ⨯ ����� � ���ᨢ�
/// \param [in, out] item - ����� ���ᨢ�, �ᯮ��㥬� � 横��
/// \param [in] arr - ��ꥪ�-���ᨢ
/// \param [in] iter - ����� ���ᨢ�, ������� �� �맮�� �����
#define for_each_array_valid(type, item, arr, iter) for_each_array(type, (item), (arr), (iter)) if((arr).valid((item)))

/// ��������� foreach ��� �������᪨� ���ᨢ�� �� ���� array_t.
/// ����஫� �� �ࠢ��쭮��� ����� ����������� �� �ணࠬ����.
/// \param [in] type - ⨯ ����� � ���ᨢ�
/// \param [in, out] item - ����� ���ᨢ�, �ᯮ��㥬� � 横��
/// \param [in] arrp - 㪠��⥫� �� ��ꥪ�-���ᨢ
/// \param [in] iter - ����� ���ᨢ�, ������� �� �맮�� �����
#define for_each_array_ptr(type, item, arrp, iter) (iter) = 0; for( type item=(type)get_null((arrp), get(iter)) ; iter < get_null((arrp), size()) ; ++iter, item=(type)get_null((arrp), get(iter)))

/// ��������� foreach ��� �������᪨� ���ᨢ�� �� ���� array_t
/// � ����஫�� �� �ࠢ��쭮��� �����.
/// \param [in] type - ⨯ ����� � ���ᨢ�
/// \param [in, out] item - ����� ���ᨢ�, �ᯮ��㥬� � 横��
/// \param [in] arrp - 㪠��⥫� �� ��ꥪ�-���ᨢ
/// \param [in] iter - ����� ���ᨢ�, ������� �� �맮�� �����
#define for_each_array_ptr_valid(type, item, arrp, iter) for_each_array_ptr(type, (item), (arrp), (iter)) if(get_null((arrp), valid((item))))


#pragma pack(pop)

#endif