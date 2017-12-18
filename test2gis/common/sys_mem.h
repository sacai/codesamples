#ifndef __SYS_MEM__
#define __SYS_MEM__

//////////////////////////////////////////////////////////////////////////////
/** \file
 	\brief �㭪樨 ��।������ ����� � ����஫�� �訡��
 	
 	�㭪樨 ��।������ ����� � ����஫�� �訡�筮 �������� ��ࠬ��஢, �訡�� �� ��������� ࠧ��� ����� � �.�.
 */

#include <stdlib.h>
#include <string.h>
#include "sys_defs.h"

#pragma pack(1)

extern "C"

{

/// �뤥����� ����� � �������� ���㫥����. ����⪠ ��� malloc. ���㫥��� �ந�������� �� ����� memset,
/// � ���� �� ������ ��⨬����� ��������஬.
/// \param [out] void **mem - 㪠��⥫� �� ��६����� ��� ࠧ��饭�� ���� �뤥������� ���ᨢ� �����
/// \param [in] size_t size - ࠧ��� �뤥�塞��� ���ᨢ� �����
/// \param [in] int zero - �ਧ��� ���㫥��� ����� (�� 㬮�砭�� �� �������)
/// \return EOK (0) � ��砥 �ᯥ�
/// \return EINVAL (22) �� �訡�� � ��室��� ������
/// \return ENOMEM (12) �� �訡�� �뤥����� �����
int malloc_r(void **mem, size_t size, int zero = 0);

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
int realloc_r(void **mem, size_t old_size, size_t new_size, int zero = 0);

/// �᢮�������� �뤥������ ����� � �������� ���㫥����
/// \param [in, out] void **mem - 㪠��⥫� �� ��६����� � ���ᮬ �᢮���������� ���ᨢ� �����
/// \param [in] size_t size - ࠧ��� �᢮���������� ���ᨢ� �����, ��� ���㫥��� (�� 㬮�砭�� �� �������)
/// \return EOK (0) � ��砥 �ᯥ�
/// \return EINVAL (22) �� �訡�� � ��室��� ������
int free_r(void **mem, size_t size = 0);

/// �㡫�஢���� ��ப� � �뤥������ ����� ��� ���, ������ strdup.
/// \param [out] char **dst - 㪠��⥫� �� ��६����� ��� ࠧ��饭�� ���� ᮧ�������� ��ப�
/// \param [in] const char *src - 㪠��⥫� �� ��ப�-���筨�
/// \return EOK (0) � ��砥 �ᯥ�
/// \return EINVAL (22) �� �訡�� � ��室��� ������
/// \return ENOMEM (12) �� �訡�� �뤥����� �����
int strdup_r(char **dst, const char *src);

};

/// ����� ��� �८�ࠧ������ 㪠��⥫�, ��।�������� � �㭪樨 �뤥����� � �᢮�������� �����
/// \param [in] mem - 㪠��⥫� �� ������� �����
#define MEM_PTR(mem) \
	(void **)&mem

/// ����� ��� �뤥����� ����� ��� ���㫥���
/// \param [in, out] mem - 㪠��⥫� �� ������� �����
/// \param [in] size - ࠧ��� ������ �����
/// \return EOK (0) � ��砥 �ᯥ�
/// \return EINVAL (22) �� �訡�� � ��室��� ������
/// \return ENOMEM (12) �� �訡�� �뤥����� �����
#define DRTSMalloc(mem, size) \
	malloc_r(MEM_PTR(mem), size, 0)

/// ����� ��� �뤥����� ����� � ���㫥����
/// \param [in, out] mem - 㪠��⥫� �� ������� �����
/// \param [in] size - ࠧ��� ������ �����
/// \return EOK (0) � ��砥 �ᯥ�
/// \return EINVAL (22) �� �訡�� � ��室��� ������
/// \return ENOMEM (12) �� �訡�� �뤥����� �����
#define DRTSMallocZero(mem, size) \
	malloc_r(MEM_PTR(mem), size, 1)

/// ����� ��� �����।������ ����� ��� ���㫥���
/// \param [in, out] mem - 㪠��⥫� �� ������� �����
/// \param [in] old_size - ⥪�騩 ࠧ��� ������ �����
/// \param [in] new_size - ���� ࠧ��� ������ �����
/// \return EOK (0) � ��砥 �ᯥ�
/// \return EINVAL (22) �� �訡�� � ��室��� ������
/// \return ENOMEM (12) �� �訡�� �뤥����� �����
#define DRTSRealloc(mem, old_size, new_size) \
	realloc_r(MEM_PTR(mem), old_size, new_size, 0)

/// ����� ��� �����।������ ����� � ���㫥����
/// \param [in, out] mem - 㪠��⥫� �� ������� �����
/// \param [in] old_size - ⥪�騩 ࠧ��� ������ �����
/// \param [in] new_size - ���� ࠧ��� ������ �����
/// \return EOK (0) � ��砥 �ᯥ�
/// \return EINVAL (22) �� �訡�� � ��室��� ������
/// \return ENOMEM (12) �� �訡�� �뤥����� �����
#define DRTSReallocZero(mem, old_size, new_size) \
	realloc_r(MEM_PTR(mem), old_size, new_size, 1)

/// ����� ��� �᢮�������� ����� ��� ���㫥���
/// \param [in, out] mem - 㪠��⥫� �� ������� �����
/// \return EOK (0) � ��砥 �ᯥ�
/// \return EINVAL (22) �� �訡�� � ��室��� ������
/// \return ENOMEM (12) �� �訡�� �뤥����� �����
#define DRTSFree(mem) \
	free_r(MEM_PTR(mem))

/// ����� ��� �᢮�������� ����� � ���㫥����
/// \param [in, out] mem - 㪠��⥫� �� ������� �����
/// \param [in] size - ࠧ��� ������ �����
/// \return EOK (0) � ��砥 �ᯥ�
/// \return EINVAL (22) �� �訡�� � ��室��� ������
/// \return ENOMEM (12) �� �訡�� �뤥����� �����
#define DRTSFreeZero(mem, size) \
	free_r(MEM_PTR(mem), size)

/// ����� ��� �㡫�஢���� ��ப�
/// \param [in, out] dst - 㪠��⥫� �� ᮧ�������� ��ப�
/// \param [in] src - 㪠��⥫� �� ��ப�-���筨�
/// \return EOK (0) � ��砥 �ᯥ�
/// \return EINVAL (22) �� �訡�� � ��室��� ������
/// \return ENOMEM (12) �� �訡�� �뤥����� �����
#define DRTSStrdup(dst, src) \
	strdup_r(&dst, src)


#pragma pack()

#endif