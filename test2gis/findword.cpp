#include "findword.h"
#include "util.h"

/** @file
  \brief ���� ��������� ᫮�� � ⥪�⮢�� 䠩��
  
  ��ꥪ� �믮���� �㭪�� ���᪠ ��������� ᫮�� � ⥪�⮢�� 䠩�� � ������ �宦����� ᫮��.
  ����� ������ ���� �����뢭�, �.�. mother != mo<crlf|tab|space|,|.|:>ther
*/

//////////////////////////////////////////////////////////////////////////////
/// ���������
/// \param char *name ��� 䠩��
/// \param int mode ०�� ������ 䠩��
/// \param int access ०�� ����㯠 � 䠩��
FindWord::FindWord(char *name, int mode, int access)
         : TextBase(name, mode, access)
{
setDivider("=:;,. \t");
setQuoter("\'\"");
word_to_find = NULL;
word_count = 0;
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// ��������
FindWord::~FindWord()
{
word_to_find = NULL;
word_count = 0;
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// �����஢���� 䠩�� � ���� ᫮��
/// \param [in] int bufsize ࠧ��� ���� �� ᪠��஢���� � ࠧ��� 䠩��
/// \param [in] char *to_find 㪠��⥫� �� ᫮�� ��� ���᪠
/// \return ������⢮ �宦����� � ��砥 �ᯥ�, 0 � ��砥 ������⢨�, -1 � ��砥 �訡�� (��� �訡�� � getError())
int FindWord::findword(int bufsize, char *to_find)
{
if(to_find != NULL)
{
    word_to_find = to_find;
    word_count = 0;
    // ������㥬 䠩�
    if(TextFile::scan(bufsize) != -1)
    {
        return word_count;
    }
	else if(getError() == EOK)
		return 0;
}
else
	setError(EINVAL);
return -1;
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// ������ ��ப� 䠩��
/// \param char *str ��ப� 䠩�� ��ࠬ��஢
/// \param int32_t pos ������ ��ப� � 䠩��
/// \param int len ����� ��ப� � 䠩��
/// \return 0 � ��砥 �ᯥ�, -1 � ��砥 �訡��, �ॡ��饩 �४�饭�� ࠧ��� 䠩��
int FindWord::parse(char *str, int32_t pos, int len)
{
if(str && *str != 0 && !isComment(*str))
{
    char *field = strtok(str);
    while(field)
    {
		if(!strcmp(field, word_to_find))
			word_count++;
        field = strtok(NULL);
    }
}
return 0;
}
///////////////////////////////////////////////////////////////////////////////

