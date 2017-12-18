#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sarray.h"

/// Функция сравнения двух элементов массива
/// \param const void* op1 номер первого элемента
/// \param const void* op2 номер второго элемента
/// \return 0 при равенстве, < 0, если op1 < op2, > 0, если op1 > op2
int compSArray(const void* op1, const void* op2)
{
	const char **str1 = (const char **)op1;
	const char **str2 = (const char **)op2;

	if(str1 && str2 && *str1 && *str2)
	{
		return(strcmp(*str1, *str2));
	}
	return 0;
}

/// Сортировка массива
/// \param нет
/// \return EOK в случае успеха
/// \return ESRCH, если массив пуст
int stringArray::sort(void)
{
	int result = ESRCH;
	char **c_arr = (char **)c_array();
	if(c_arr && size() > 0)
	{
		Lock();
		qsort(c_arr, size(), sizeof(char *), compSArray);
		UnLock();
		result = EOK;
	}
	
	setError(result);
	return(result);
}

/// Линейный поиск элемента
/// \param [in] char *item значение строки для поиска
/// \return значение элемента, либо NULL в случае ошибки
char* stringArray::find(char *item)
{
	char **c_arr = (char **)c_array();
	char *result = NULL;
	if(c_arr && size())
	{
		Lock();
		for(int i = 0; i < size(); i++)
		{
			if(valid(c_arr[i]) && !strcmp(c_arr[i], item))
			{
				result = c_arr[i];
				break;
			}
		}
		UnLock();
	}
	if(result)
		setError(EOK);
	else
		setError(ESRCH);
	return(result);
}

/// Функция сравнения двух элементов массива для поиска
/// \param const void* op1 номер первого элемента
/// \param const void* op2 номер второго элемента
/// \return 0 при равенстве, < 0, если op1 < op2, > 0, если op1 > op2
int binSArray(const void* op1, const void* op2)
{
	const char *str1 = (const char *)op1;
	const char **str2 = (const char **)op2;

	if(str1 && str2 && *str2)
	{
		return(strcmp(str1, *str2));
	}
	return 0;
}

/// Двоичный поиск в массиве
/// \param [in] char *item искомое значение
/// \return значение элемента, либо NULL в случае ошибки
char* stringArray::bfind(char *item)
{
	char **c_arr = (char**)c_array();
	char *result = NULL;

	if(c_arr && size())
	{
		Lock();
		char **keyp = (char **)bsearch(item, c_arr, size(), sizeof(char *), binSArray);
		if(keyp && *keyp)
			result = *keyp;
		UnLock();
	}

	if(result)
		setError(EOK);
	else
		setError(ESRCH);
	return(result);
}

