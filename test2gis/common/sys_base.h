#ifndef __SYS_BASE_CLASS__
#define __SYS_BASE_CLASS__

/** \file
 \brief ������ ����� ��� ��� ��ꥪ⮢ ��⥬�

	������ ����� ��� �ᥧ ��ꥪ⮢ ��⥬�. �࠭�� �ਧ��� �訡��, ����㯭� � �ந������� ����� �१ �㭪樨

*/

#include <errno.h>
#include "sys_defs.h"

#pragma pack(push,1)

extern "C" {

/// ������� ����� ��� ��� ��ꥪ⮢ ��⥬�
class DRTS_Base_Object
{
 volatile uint32_t ei;          ///< ����騩 �ਧ��� �訡��
public:

 /// ��⠭����� �ਧ��� �訡��
 /// \param [in] uint32_t err ���祭�� �ਧ���� �訡��
 /// \return ���
 void setError(uint32_t err) {ei=err;};

 /// ������ �ਧ��� �訡��
 /// \param [in] uint32_t err ���祭�� �ਧ���� �訡��
 /// \return ���
 void clearError(uint32_t err) {ei=EOK;};

 /// ������ �ਧ��� �訡��
 /// \param ���
 /// \return ���祭�� �ਧ���� �訡��
 uint32_t getError() {return ei;};

 /// ����������
 /// \param ���
 DRTS_Base_Object() {ei=EOK;};

 /// ��������
 /// \param ���
 ~DRTS_Base_Object() {ei=EOK;};

};

};

#pragma pack(pop)

#endif