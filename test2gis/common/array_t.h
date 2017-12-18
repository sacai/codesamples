#ifndef __DRTS_ARRAY_T__
#define __DRTS_ARRAY_T__

/** \file
	\brief Шаблон абстрактного динамического массива

	Шаблон абстрактного динамического массива. Используется как основа для массива,
	предназначенного для хранения однотипных элементов с возможностью их добавления,
	вставки в определенную позицию и удаления. Легковесная замена стандартному
	вектору, гарантирующая, что память не перераспределяется при доступе к элементам
	массива и, таким образом, допускающая использование в обработчиках сигналов и
	прерываний.
	
	Может быть также использован в качестве базового класса при создании массивов
	со специфичными функциями.

*/

#pragma pack(push,1)

#include <stdint.h>
#include "sys_base.h"
#include "sys_mem.h"
#include "sys_atom.h"

/// Шаблон абстрактного динамического массива
template <class T>
class array_t : public DRTS_Base_Object
{
	T *arr;						///< Массив элементов заданного типа
	size_t a_delta;				///< Приращение
	size_t a_nfree;				///< Кол-во свободных элементов
	size_t a_size;				///< Кол-во используемых элементов
	size_t a_capacity;			///< Максимальное количество элементов в массиве
	atomic_uint32_t in_use;		///< Признак блокировки массива
	T invalid_value;			///< Недопустимое значение элемента
	
public:
	/// Конструктор по умолчанию
	array_t()
	{
		init();
	};

	/// Конструктор
	/// \param [in] size_t startsize исходный размер массива
	/// \param [in] size_t delta количество элементов, на которое увеличивается размер
	/// массива и перераспределяется память при исчерпании свободного места (по умолчанию
	/// приравнивается половине исходного размера)
	array_t(size_t startsize, size_t delta = 0)
	{
		init();
		init(startsize, delta);
	};

	/// Конструктор копирования
	/// \param [in] const array_t<T> &src исходный массив
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

	/// Деструктор
	~array_t()
	{
		clear();
	};

	/// Инициализация массива
	/// \param [in] size_t startsize исходный размер массива
	/// \param [in] size_t delta количество элементов, на которое увеличивается размер
	/// массива и перераспределяется память при исчерпании свободного места (по умолчанию
	/// приравнивается половине исходного размера)
	/// \return EOK (0) в случае успеха
	/// \return EINVAL (22) при неверных параметрах
	/// \return ENOMEM (12) при ошибке выделения памяти
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

	/// Инициализация пустого массива
	/// \param нет
	/// \return EOK (0) в случае успеха
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

	/// Очистка массива с освобождением памяти
	/// \param нет
	/// \return нет
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

	/// Добавление элемента в массив
	/// \param [in] T item добавляемый элемент
	/// \param [in] bool lck блокировать ли массив (true) или нет (false),
	/// по умолчанию блокировать
	/// \return EOK (0) в случае успеха
	/// \return ENOMEM (12) при ошибке выделения памяти
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

	/// Добавление элемента в массив с его созданием
	/// \param [in] bool lck блокировать ли массив (true) или нет (false),
	/// по умолчанию блокировать
	/// \return EOK (0) в случае успеха
	/// \return ENOMEM (12) при ошибке выделения памяти
	int add(bool lck = true) {return(add(make_empty(), lck));};

	/// Создание пустого элемента. Функция может быть переопределена в производном классе.
	/// \param нет
	/// \return неинициализированный элемент массива (0)
	/// \return invalid() в случае ошибки
	/// (признак ошибки должен быть возвращен в getError())
	virtual T make_empty(void) {T item = {0}; return(item);};

	/// Создание элемента. Функция может быть переопределена в производном классе.
	/// \param [in] T src исходное значение элемента
	/// \return неинициализированный элемент массива (0)
	/// \return invalid() в случае ошибки
	/// (признак ошибки должен быть возвращен в getError())
	virtual T make_item(T src) {T item = src; return(item);};
	
	/// Вставка элемента в массив на заданную позицию
	/// \param [in] T item добавляемый элемент
	/// \param [in] size_t pos позиция
	/// \param [in] bool lck блокировать ли массив (true) или нет (false),
	/// по умолчанию блокировать
	/// \return EOK (0) в случае успеха
	/// \return ENOMEM (12) при ошибке выделения памяти
	/// \return ENOENT (2), если позиция выходит за пределы массива
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

	/// Присвоение значения элемента на заданной позиции
	/// \param [in] size_t pos позиция
	/// \param [in] T item значение элемента
	/// \param [in] bool lck блокировать ли массив (true) или нет (false),
	/// по умолчанию блокировать
	/// \return EOK (0) в случае успеха
	/// \return ENOENT (2), если позиция выходит за пределы массива
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

	/// Доступ к элементу массива
	/// \param [in] size_t index позиция
	/// \param [in] bool lck блокировать ли массив (true) или нет (false),
	/// по умолчанию блокировать
	/// \return invalid() в случае ошибки
	/// (признак ошибки должен быть возвращен в getError())
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

	/// Доступ к элементу массива (оператор [])
	/// \param [in] size_t index позиция
	/// \return invalid() в случае ошибки
	/// (признак ошибки должен быть возвращен в getError())
	T operator[](size_t index) {return(get(index));};

	/// Доступ к первому элементу массива
	/// \param нет
	/// \return invalid() в случае ошибки
	/// (признак ошибки должен быть возвращен в getError())
	T getfirst(void)
	{
		if(a_size) return(get(0));
		setError(ENOENT);
		return invalid();
	};

	/// Доступ к последнему элементу массива
	/// \param нет
	/// \return invalid() в случае ошибки
	/// (признак ошибки должен быть возвращен в getError())
	T getlast(void)
	{
		if(a_size) return(get(a_size - 1));
		setError(ENOENT);
		return invalid();
	};

	/// Очистка элемента. Функция может быть переопределена в производном классе.
	/// \param [in] T &item ссылка на очищаемый элемент
	/// \return EOK (0) в случае успеха
	/// \return ENOMEM (12) при ошибке освобождения памяти
	virtual int clean_item(T &item) {item = {0}; setError(EOK); return(EOK);};

	/// Удаление указанного элемента
	/// \param [in] size_t pos позиция
	/// \param [in] bool lck блокировать ли массив (true) или нет (false),
	/// по умолчанию блокировать
	/// \param [in] bool clean очищать ли элемент (true) или нет (false),
	/// по умолчанию очищать
	/// \return EOK (0) в случае успеха
	/// \return ENOENT (2), если позиция выходит за пределы массива
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

	/// Удаление последнего элемента
	/// \param [in] bool lck блокировать ли массив (true) или нет (false),
	/// по умолчанию блокировать
	/// \param [in] bool clean очищать ли элемент (true) или нет (false),
	/// по умолчанию очищать
	/// \return EOK (0) в случае успеха
	/// \return ENOENT (2), если массив пуст
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

	/// Очистка массива без освобождения памяти, выделенной под массив
	/// \param [in] bool lck блокировать ли массив (true) или нет (false),
	/// по умолчанию блокировать
	/// \param [in] bool clean очищать ли элементы (true) или нет (false),
	/// по умолчанию очищать
	/// \return EOK (0) в случае успеха
	/// \return ENOENT (2), если массив пуст
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

	/// Блокировка массива от изменений
    /// \param [in] int tries - максимальное количество попыток (по умолчанию 10)
    /// \return EOK (0) в случае успеха
    /// \return EBUSY (16) при невозможности блокировки
	int Lock(int tries = 10)
	{
		int result = AtomicLockSwitch(&in_use, 0U, 1U, tries);
		setError(result);
		return(result);
	};

	/// Получение признака блокировки массива
    /// \param нет
    /// \return значение признака блокировки (1 или 0)
	uint32_t GetUsed(void) {return(AtomicGet(&in_use));};

	/// Снятие блокировки массива
    /// \param [in] int tries - максимальное количество попыток (по умолчанию 10)
    /// \return EOK (0) в случае успеха
    /// \return EBUSY (16) при невозможности блокировки
    /// \return EINVAL (22), если массив не заблокирован
	int UnLock(int tries = 10)
	{
		int result = EINVAL;
		if(GetUsed())
			result = AtomicLockSwitch(&in_use, 1U, 0U, tries);
		setError(result);
		return(result);
	};

	/// Получить текущий размер массива
	/// \param нет
	/// \return размер массива
	size_t size() {return(a_size);};

	/// Получить максимальный размер массива
	/// \param нет
	/// \return максимальный размер массива
	size_t capacity() {return(a_capacity);};

	/// Получить непосредственный доступ к массиву (например, для сортировки)
	/// \param нет
	/// \return указатель на массив или NULL, если массив не инициализирован
	/// (после вызова конструктора по умолчанию или clear())
	const T* c_array() {return(arr);};

	/// Определить, является ли допустимым элемент массива
	/// \param [in] item проверяемое значение элемента
	/// \return true, если элемент является допустимым
	/// \return false, если не является
	bool valid(T item) {return(item != invalid());};

	/// Возвращает недопустимое значение элемента массива (по умолчанию -1).
	/// Может быть переопределена в производном классе, например. если
	/// недопустимым значением является нечто другое
	/// \param нет
	/// \return недопустимое значение элемента
	T invalid(void) {return(invalid_value);};

	/// Установить недопустимое значение элемента массива
	/// \param [in] value недопустимое значение элемента
	/// \return нет
	void set_invalid(T value) {invalid_value = value;};

};

/// Реализация foreach для динамических массивов на базе array_t.
/// Контроль на правильность элемента возлагается на программиста.
/// \param [in] type - тип элемента в массиве
/// \param [in, out] item - элемент массива, используемый в цикле
/// \param [in] arr - объект-массив
/// \param [in] iter - итератор массива, объявляется до вызова макроса
#define for_each_array(type, item, arr, iter) (iter) = 0; for( type item=(type)(arr).get(iter) ; iter < (arr).size() ; ++iter, item=(type)(arr).get(iter))

/// Реализация foreach для динамических массивов на базе array_t
/// с контролем на правильность элемента.
/// \param [in] type - тип элемента в массиве
/// \param [in, out] item - элемент массива, используемый в цикле
/// \param [in] arr - объект-массив
/// \param [in] iter - итератор массива, объявляется до вызова макроса
#define for_each_array_valid(type, item, arr, iter) for_each_array(type, (item), (arr), (iter)) if((arr).valid((item)))

/// Реализация foreach для динамических массивов на базе array_t.
/// Контроль на правильность элемента возлагается на программиста.
/// \param [in] type - тип элемента в массиве
/// \param [in, out] item - элемент массива, используемый в цикле
/// \param [in] arrp - указатель на объект-массив
/// \param [in] iter - итератор массива, объявляется до вызова макроса
#define for_each_array_ptr(type, item, arrp, iter) (iter) = 0; for( type item=(type)get_null((arrp), get(iter)) ; iter < get_null((arrp), size()) ; ++iter, item=(type)get_null((arrp), get(iter)))

/// Реализация foreach для динамических массивов на базе array_t
/// с контролем на правильность элемента.
/// \param [in] type - тип элемента в массиве
/// \param [in, out] item - элемент массива, используемый в цикле
/// \param [in] arrp - указатель на объект-массив
/// \param [in] iter - итератор массива, объявляется до вызова макроса
#define for_each_array_ptr_valid(type, item, arrp, iter) for_each_array_ptr(type, (item), (arrp), (iter)) if(get_null((arrp), valid((item))))


#pragma pack(pop)

#endif