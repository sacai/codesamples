//////////////////////////////////////////////////////////////////////////////
/** \file
 	\brief �㭪樨 ��।������ ����� � ����஫�� �訡��
 	
 	�㭪樨 ��।������ ����� � ����஫�� �訡�筮 �������� ��ࠬ��஢, �訡�� �� ��������� ࠧ��� ����� � �.�.
 */

#include "sys_mem.h"

//////////////////////////////////////////////////////////////////////////////
/// �뤥����� ����� � �������� ���㫥����. ����⪠ ��� malloc. ���㫥��� �ந�������� �� ����� memset,
/// � ���� �� ������ ��⨬����� ��������஬.
/// \param [out] void **mem - 㪠��⥫� �� ��६����� ��� ࠧ��饭�� ���� �뤥������� ���ᨢ� �����
/// \param [in] size_t size - ࠧ��� �뤥�塞��� ���ᨢ� �����
/// \param [in] int zero - �ਧ��� ���㫥��� ����� (�� 㬮�砭�� �� �������)
/// \return EOK (0) � ��砥 �ᯥ�
/// \return EINVAL (22) �� �訡�� � ��室��� ������
/// \return ENOMEM (12) �� �訡�� �뤥����� �����
int malloc_r(void **mem, size_t size, int zero)
{
int result = EINVAL;
void *tmp = NULL;

if(mem && size)
{
	tmp = malloc(size);
	if(tmp)
	{
		if(zero)
			memset(tmp, 0, size);
		result = EOK;
	}
	else
		result = ENOMEM;
	*mem = tmp;
}

return(result);
}
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
/// �����।������ ����� � �������� ���㫥���� ������������ ����࠭�⢠. ����஫������
/// �����頥��� ���祭�� �⠭���⭮� ��樨 realloc, � ⥬, �⮡� �� ������� 㠧�⥫� �� ࠭�� �뤥������
/// ������.
/// \param [in, out] void **mem - 㪠��⥫� �� ��६����� ��� ࠧ��饭�� ���� �����।�������� ���ᨢ� �����
/// \param [in] size_t old_size - ⥪�騩 ࠧ��� ���ᨢ� �����
/// \param [in] size_t new_size - ���� ࠧ��� ���ᨢ� �����
/// \param [in] int zero - �ਧ��� ���㫥��� ����� (�� 㬮�砭�� �� �������)
/// \return EOK (0) � ��砥 �ᯥ�
/// \return EINVAL (22) �� �訡�� � ��室��� ������
/// \return ENOMEM (12) �� �訡�� �����।������ �����
int realloc_r(void **mem, size_t old_size, size_t new_size, int zero)
{
int result = EINVAL;
void *tmp = NULL;
byte_t *to_zero = NULL;

if(mem && *mem && old_size && new_size)
{
	tmp = *mem;
	tmp = realloc(tmp, new_size);
	if(tmp)
	{
		*mem = tmp;
		if(new_size > old_size && zero)
		{
			to_zero = (byte_t *)tmp + old_size;
			memset(to_zero, 0, new_size - old_size);
		}
		result = EOK;
	}
	else
		result = ENOMEM;
}

return(result);
}
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
/// �᢮�������� �뤥������ ����� � �������� ���㫥����
/// \param [in, out] void **mem - 㪠��⥫� �� ��६����� � ���ᮬ �᢮���������� ���ᨢ� �����
/// \param [in] size_t size - ࠧ��� �᢮���������� ���ᨢ� �����, ��� ���㫥��� (�� 㬮�砭�� �� �������)
/// \return EOK (0) � ��砥 �ᯥ�
/// \return EINVAL (22) �� �訡�� � ��室��� ������
int free_r(void **mem, size_t size)
{
int result = EINVAL;
void *tmp = NULL;

if(mem && *mem)
{
	tmp = *mem;
	if(size)
		memset(tmp, 0, size);
	free_null(tmp);
	*mem = tmp;
	result = EOK;
}

return(result);
}
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
/// �㡫�஢���� ��ப� � �뤥������ ����� ��� ���, ������ strdup.
/// \param [out] char **dst - 㪠��⥫� �� ��६����� ��� ࠧ��饭�� ���� ᮧ�������� ��ப�
/// \param [in] const char *src - 㪠��⥫� �� ��ப�-���筨�
/// \return EOK (0) � ��砥 �ᯥ�
/// \return EINVAL (22) �� �訡�� � ��室��� ������
/// \return ENOMEM (12) �� �訡�� �뤥����� �����
int strdup_r(char **dst, const char *src)
{
int result = EINVAL;
char *tmp = NULL;
size_t len = 0;

if(dst && src)
{
	len = strlen(src);
	result = DRTSMallocZero(tmp, len+1);
	if(result == EOK)
	{
		if(len)
			strncpy(tmp, src, len+1);
		*dst = tmp;
	}
}

return(result);
}
//////////////////////////////////////////////////////////////////////////////
