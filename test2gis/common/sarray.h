#if !defined (__SARRAY_H)
#define __SARRAY_H
/** \file
 \brief Реализация динамического массива строк
 
  Реализация класса "массив строк",
  предлагающего средства хранения массива строк c
  автоматическим увеличением размера массива при
  превышении задаваемой границы.

*/

#include "array_t.h"

#pragma pack(push,1)

extern "C" {

/// Динамический массив строк
class stringArray : public array_t<char*>
{
public:

	/// Конструктор по умолчанию
	/// \param нет
	stringArray() : array_t<char*>()
	 {set_invalid((char *)NULL);};

	/// Конструктор
	/// \param [in] size_t startsize исходный размер массива
	/// \param [in] size_t delta количество элементов, на которое увеличивается размер
	/// массива и перераспределяется память при исчерпании свободного места (по умолчанию
	/// приравнивается половине исходного размера)
	stringArray(size_t startsize, size_t delta = 0) : array_t<char*>(startsize, delta)
	 {set_invalid((char *)NULL);};

	/// Конструктор копирования
	/// \param [in] const s_array &src исходный массив
	stringArray(const stringArray &src) : array_t<char*>(src) 
	{
		for(int i = 0; i < size(); i++)
		{
			char *item = get(i);
			set(item, i);
		}
	};

	/// Деструктор
	~stringArray() {flush();};
	
	/// Создание элемента
	/// \param [in] char *src исходное значение элемента
	/// \return новый элемент массива
	/// \return invalid() в случае ошибки
	/// (признак ошибки должен быть возвращен в getError())
	char* make_item(char *src)
	{
		char *item = invalid();
		if(src)
			setError(DRTSStrdup(item, src));
		return(item);
	};
	
	/// Очистка элемента
	/// \param [in] char* &item ссылка на очищаемый элемент
	/// \return EOK (0) в случае успеха
	/// \return ENOMEM (12) при ошибке освобождения памяти
	int clean_item(char* &item)
	{
		int result = EINVAL;
		if(item != invalid())
			DRTSFree(item);
		setError(result);
		return(result);
	};

	/// Добавление элемента в конец массива в виде оператора +=
	/// \param [in] char* c добавляемое значение
	/// \return ссылка на данный объект
	stringArray& operator+=(char* c)  {add(c); return *this;};
	stringArray& operator+=(const char* c)  {add((char *)c); return *this;};

	/// Добавление элемента в конец массива в виде оператора +
	/// \param [in] char* c добавляемое значение
	/// \return ссылка на данный объект
	stringArray& operator+(char* c)  {add(c); return *this;};
	stringArray& operator+(const char* c)  {add((char *)c); return *this;};

	/// Удаление последнего элемента в виде оператора +-
	/// \param нет
	/// \return ссылка на данный объект
	stringArray& operator--() { removelast(); return *this;};

	/// Сортировка массива
	/// \param нет
	/// \return EOK в случае успеха
	/// \return ESRCH, если массив пуст
	int sort(void);

	/// Линейный поиск элемента
	/// \param [in] char *item значение строки для поиска
	/// \return значение элемента, либо NULL в случае ошибки
	char* find(char *item);
	char* find(const char *item) {return(find((char *)item));};

	/// Двоичный поиск в массиве
	/// \param [in] char *item искомое значение
	/// \return значение элемента, либо NULL в случае ошибки
	char* bfind(char *item);
	char* bfind(const char *item) {return(bfind((char *)item));};

};

typedef stringArray sArray;

/// Реализация foreach для динамического массива строк sArray.
/// Контроль на неравенство элемента NULL возлагается на программиста.
/// \param [in, out] item - элемент массива, используемый в цикле
/// \param [in] arrp - указатель на объект-массив
/// \param [in] iter - итератор массива, объявляется до вызова макроса
#define for_each_sarray(item, arrp, iter) for_each_array_ptr(char *, (item), (arrp), (iter))

/// Реализация foreach для динамического массива строк sArray с контролем на неравенство элемента NULL.
/// \param [in, out] item - элемент массива, используемый в цикле
/// \param [in] arrp - указатель на объект-массив
/// \param [in] iter - итератор массива, объявляется до вызова макроса
#define for_each_sarray_null(item, arrp, iter) for_each_array_ptr_valid(char *, (item), (arrp), (iter))

};

#pragma pack(pop)

#endif