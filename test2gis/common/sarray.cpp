#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sarray.h"

/// ������� ��������� ���� ��������� �������
/// \param const void* op1 ����� ������� ��������
/// \param const void* op2 ����� ������� ��������
/// \return 0 ��� ���������, < 0, ���� op1 < op2, > 0, ���� op1 > op2
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

/// ���������� �������
/// \param ���
/// \return EOK � ������ ������
/// \return ESRCH, ���� ������ ����
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

/// �������� ����� ��������
/// \param [in] char *item �������� ������ ��� ������
/// \return �������� ��������, ���� NULL � ������ ������
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

/// ������� ��������� ���� ��������� ������� ��� ������
/// \param const void* op1 ����� ������� ��������
/// \param const void* op2 ����� ������� ��������
/// \return 0 ��� ���������, < 0, ���� op1 < op2, > 0, ���� op1 > op2
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

/// �������� ����� � �������
/// \param [in] char *item ������� ��������
/// \return �������� ��������, ���� NULL � ������ ������
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

