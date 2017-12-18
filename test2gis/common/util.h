#ifndef _UTIL_
#define _UTIL_

/** \file
	\brief	������� �ࢨ�� �㭪樨 ��� �ᯮ�짮����� ��� ����� ������⥪�, ⠪ � � �������� ������

*/

#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "sys_defs.h"

extern "C" {

///////////////////////////////////////////////////////////////////////////////
/// ��१���� �஡���� � ��砫� � ���� ��ப�
/// \param [in, out] char *string - ��ப� ��� ��ࠡ�⪨
/// \return 㪠��⥫� �� ��ப� ��� �஡���� � ��砫� � � ����
char *Trim(char *string);

///////////////////////////////////////////////////////////////////////////////
/// �����頥� ��� 䠩�� ��� ���
/// \param [in] char *Path - ����� ���� � ��� 䠩��
/// \return 㪠��⥫� �� ��� 䠩��
char* PointToName(char *Path);

///////////////////////////////////////////////////////////////////////////////
/// ����⠭���� ��६����� ���㦥��� � ��ப�
/// \param [in]
/// char *src - ��ப�, ᮤ�ঠ�� ��६���� ���㦥��� � ���� $��६�����
/// \param [out]
/// char *dst - ���� ��� ��ப�, ���७��� ���祭�ﬨ ��६����� ���㦥���
/// \param [in]
/// int dstSize - ࠧ��� ���� � �����
/// \param [in]
/// char *quotestr - ��ப� ᨬ�����. ����� ����� ���� �ᯮ�짮���� � ����⢥
///                  ��࠭��⥫��, �� 㬮�砭�� �ᯮ������ ᨬ��� '$'
/// \return ����� ��ப�-१����, ���� 0 � ��砥 �訡��
int ExpandEnv(char *src, char *dst, int dstSize, char *quotestr = NULL);

///////////////////////////////////////////////////////////////////////////////
/// �������� ��� (\ ��� /) � ��ப� ��� 䠩����� ��⥬�
/// \param [in, out] char *Path - ��ப� ���
/// \param [in] int reverse - ��אַ� (0) ��� ����� (1) ���
/// \return ���
void AddEndSlash(char *Path, int reverse = 0);

///////////////////////////////////////////////////////////////////////////////
/// ����祭�� ࠧ��� 䠩�� �� ���ਯ��� (� �ᯮ�짮������ lseek)
/// \param [in]
/// int handle - ���ਯ�� 䠩��
/// \return
/// ࠧ��� 䠩��, ���� -1 � ��砥 �訡��
off_t GetFileSizeByHandle(int handle);

///////////////////////////////////////////////////////////////////////////////
/// ����祭�� ࠧ��� 䠩�� �� ���ਯ��� (� �ᯮ�짮������ lseek) � �����⮬ �ਧ���� �訡��
/// \param [in]
/// int handle - ���ਯ�� 䠩��
/// \param [out]
/// off_t *size - ࠧ��� 䠩��
/// \return
/// EOK (0), ���� �ਧ��� �訡��
int GetFileSizeByHandleEx(int handle, off_t *size);

///////////////////////////////////////////////////////////////////////////////
/// ����祭�� ࠧ��� 䠩�� �� ����� (� �ᯮ�짮������ lseek)
/// \param [in]
/// char *name - ��� 䠩��
/// \return
/// ࠧ��� 䠩��, ���� -1 � ��砥 �訡��
off_t GetFileSizeByName(char *name);

///////////////////////////////////////////////////////////////////////////////
/// ����祭�� ࠧ��� 䠩�� �� ����� (� �ᯮ�짮������ lseek) � �����⮬ �ਧ���� �訡��
/// \param
/// char *name - ��� 䠩��
/// \param [out]
/// off_t *size - ࠧ��� 䠩��
/// \return
/// EOK (0), ���� �ਧ��� �訡��
int GetFileSizeByNameEx(char *name, off_t *size);

///////////////////////////////////////////////////////////////////////////////
/// �ࠢ����� ��ப ��� ��� ॣ����
/// \param [in] const char *str1 - ����� ���� ��ࠢ���⢠
/// \param [in] const char *str2 - �ࠢ�� ���� ��ࠢ���⢠
/// \return < 0, �᫨ str1 < str2
/// \return 0, �᫨ str1 == str2
/// \return > 0, �᫨ str1 > str2
int Stricmp(const char *str1, const char *str2);

///////////////////////////////////////////////////////////////////////////////
/// �ࠢ����� ��ப ��� ��� ॣ���� �� ����� 祬 ��������� ������⢠ ᨬ�����
/// \param [in] const char *str1 - ����� ���� ��ࠢ���⢠
/// \param [in] const char *str2 - �ࠢ�� ���� ��ࠢ���⢠
/// \param [in] size_t count - ������⢮ �ࠢ�������� ᨬ�����
/// \return < 0, �᫨ str1 < str2
/// \return 0, �᫨ str1 == str2
/// \return > 0, �᫨ str1 > str2
int Strnicmp(const char *str1, const char *str2, size_t count);

};

#endif