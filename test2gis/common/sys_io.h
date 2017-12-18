#ifndef __SYS_IO__
#define __SYS_IO__

//////////////////////////////////////////////////////////////////////////////
/** \file
 	\brief �㭪樨 �⥭��/����� � 䠩�, � ����஫�� ���뢠��� �� ᨣ����
 	
 	�㭪樨 �⥭��/����� � 䠩�, � ����஫�� ���뢠��� �� ᨣ���� � �����⮬ � 横� ������/�⥭��/�����
 */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "sys_defs.h"

#pragma pack(push,1)

#ifndef O_BINARY
#define O_BINARY	0
#endif

#ifndef O_TEXT
#define O_TEXT		0
#endif

extern "C"

{

/// ����⨥ 䠩�� � ����஫�� ���뢠��� �� ᨣ����
/// \param [out] int *handle - 㪠��⥫� �� ��६����� ��� ���ਯ�� 䠩��
/// \param [in] char *name - ��� 䠩��
/// \param [in] int mode - ०�� ������ 䠩��
/// \param [in] int access - ��᪠ �ࠢ �� ����� � 䠩��
/// \return EOK (0) � ��砥 �ᯥ�
/// \return EINVAL (22), �᫨ 㪠��⥫� �� ���樠����஢���
/// \return �訡�� ������
int open_r(int *handle, char *name, int mode, int access = 0777);

/// �⥭�� 䠩�� � ����஫�� ���뢠��� �� ᨣ����
/// \param [in] int handle - ���ਯ�� 䠩��
/// \param [out] void *buffer - 㪠��⥫� �� ���� ��� �⥭�� �� 䠩�
/// \param [in] size_t size - ࠧ��� ���� ��� ����� � 䠩�
/// \param [out] off_t *r_bytes - 㪠��⥫� �� ��६����� ��� ������⢮ ���⠭��� ����
/// \return EOK (0) � ��砥 �ᯥ�
/// \return ENOENT (2), �᫨ 䠩� �� �����
/// \return EINVAL (22), �᫨ 㪠��⥫� �� ���� �� ���樠����஢��
/// \return �訡�� �⥭��, �᫨ �� 㤠���� ������ ����室���� ������⢮ ������, � �� �⮬ �⥭�� �� ���뢠���� ᨣ�����
int read_r(int handle, void *buffer, size_t size, off_t *r_bytes = NULL);

/// ������ 䠩�� � ����஫�� ���뢠��� �� ᨣ����
/// \param [in] int handle - ���ਯ�� 䠩��
/// \param [in] void *buffer - 㪠��⥫� �� ���� ��� ����� � 䠩�
/// \param [in] size_t size - ࠧ��� ���� ��� ����� � 䠩�
/// \param [out] off_t *w_bytes - 㪠��⥫� �� ��६����� ��� ������⢮ ����ᠭ��� ����
/// \return EOK (0) � ��砥 �ᯥ�
/// \return ENOENT (2), �᫨ 䠩� �� �����
/// \return EINVAL (22), �᫨ 㪠��⥫� �� ���� �� ���樠����஢��
/// \return �訡�� �����, �᫨ �� 㤠���� ������� ����室���� ������⢮ ������, � �� �⮬ �⥭�� �� ���뢠���� ᨣ�����
int write_r(int handle, void *buffer, size_t size, off_t *w_bytes = NULL);

/// �����⨥ 䠩�� � ����஫�� ���뢠��� �� ᨣ����
/// \param [in] int handle - ���ਯ�� 䠩��
/// \return EOK (0) � ��砥 �ᯥ�
/// \return �訡�� �������
int close_r(int handle);

};

#pragma pack(pop)

#endif