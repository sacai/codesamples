#ifndef _CHECKSUM_
#define _CHECKSUM_

/** \file
  \brief ������ �����஫쭮� �㬬� 䠩��
  
  ��ꥪ� �믮���� �㭪�� ������ ����஫쭮� �㬬� 䠩�� �� �������
  checksum = word1 + word2 + ... + wordN,
  ��� word - 32��⭮� ���祭�� �� 䠩��
*/

#include "textbase.h"

#pragma pack(push,1)

/// �ਧ��� �訡��
#define BAD_CHECKSUM	0xFFFFFFFF

///////////////////////////////////////////////////////////////////////////////
/// ������ ����஫쭮� �㬬� 䠩��
/// �� ���� ��ꥪ� ��� ࠧ��� ⥪�⮢�� 䠩���               
class CheckSum : public TextFile
{
	uint32_t check_sum;           ///< ����஫쭠� �㬬�

public:

    /// ���������
    /// \param [in] char *name ��� 䠩��
    /// \param [in] int mode ०�� ������ 䠩��
    /// \param [in] int access ०�� ����㯠 � 䠩��
    CheckSum(char *name, int mode=O_RDONLY, int access=0777);

    /// ��������
    /// \param ���
    ~CheckSum();

    /// �����஢���� 䠩�� � ������ �㬬�
    /// \param ���
    /// \return ����஫쭠� �㬬� � ��砥 �ᯥ�, 0xFFFFFFFF � ��砥 �訡�� (��� �訡�� � getError())
    uint32_t get_checksum();

    /// ������ ����� 䠩��
    /// \param [in] char *str 㪠��⥫� �� ���� 䠩��
    /// \param [in] int32_t pos ������ ��ப� � 䠩�� (����������)
    /// \param [in] int len ������⢮ ᨬ����� � �����
    /// \return 0 � ��砥 �ᯥ�, -1 � ��砥 �訡��, �ॡ��饩 �४�饭�� ࠧ��� 䠩��
    virtual int parse(char *str, int32_t pos, int len);

};

#pragma pack(pop)


#endif