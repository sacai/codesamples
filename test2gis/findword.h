#ifndef _FINDWORD_
#define _FINDWORD_

/** \file
  \brief ���� ��������� ᫮�� � ⥪�⮢�� 䠩��
  
  ��ꥪ� �믮���� �㭪�� ���᪠ ��������� ᫮�� � ⥪�⮢�� 䠩��.
  ����� ������ ���� �����뢭�, �.�. mother != mo<crlf|tab|space|,|.|:>ther
*/

#include "textbase.h"

#pragma pack(push,1)

///////////////////////////////////////////////////////////////////////////////
/// ���� ᫮�� � ⥪�⮢�� 䠩�� � ������ ��� �宦�����
/// �� ���� ��ꥪ� ��� ࠧ��� �������஢����� ⥪�⮢�� 䠩���               
class FindWord : public TextBase
{
    char *word_to_find;                ///< �����⥫� �� �᪮��� ᫮��
	unsigned int word_count;           ///< ���稪 �宦����� ᫮��

public:

    /// ���������
    /// \param [in] char *name ��� 䠩��
    /// \param [in] int mode ०�� ������ 䠩��
    /// \param [in] int access ०�� ����㯠 � 䠩��
    FindWord(char *name, int mode=O_RDONLY, int access=0777);

    /// ��������
    /// \param ���
    ~FindWord();

    /// �����஢���� 䠩�� � ���� ᫮��
    /// \param [in] int bufsize ࠧ��� ���� �� ᪠��஢���� � ࠧ��� 䠩��
    /// \param [in] char *to_find 㪠��⥫� �� ᫮�� ��� ���᪠
    /// \return ������⢮ �宦����� � ��砥 �ᯥ�, 0 � ��砥 ������⢨�, -1 � ��砥 �訡�� (��� �訡�� � getError())
    int findword(int bufsize, char *to_find);

    /// ������ ��ப� 䠩��
    /// \param [in] char *str ��ப� 䠩��
    /// \param [in] int32_t pos ������ ��ப� � 䠩��
    /// \param [in] int len ����� ��ப� � 䠩��
    /// \return 0 � ��砥 �ᯥ�, -1 � ��砥 �訡��, �ॡ��饩 �४�饭�� ࠧ��� 䠩��
    virtual int parse(char *str, int32_t pos, int len);

};

#pragma pack(pop)


#endif