#include "checksum.h"

/** @file
  \brief ������ ����஫쭮� �㬬� 䠩��
  
  ��ꥪ� �믮���� �㭪�� ������ ����஫쭮� �㬬� 䠩�� �� �������
  checksum = word1 + word2 + ... + wordN,
  ��� word - 32��⭮� ���祭�� �� 䠩��
*/

//////////////////////////////////////////////////////////////////////////////
/// ���������
/// \param char *name ��� 䠩��
/// \param int mode ०�� ������ 䠩��
/// \param int access ०�� ����㯠 � 䠩��
CheckSum::CheckSum(char *name, int mode, int access)
         : TextFile(name, mode, access)
{
check_sum = 0;
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// ��������
CheckSum::~CheckSum()
{
check_sum = 0;
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// �����஢���� 䠩�� � ������ �㬬�
/// \param ���
/// \return ����஫쭠� �㬬� � ��砥 �ᯥ�, 0xFFFFFFFF � ��砥 �訡�� (��� �訡�� � getError())
uint32_t CheckSum::get_checksum()
{
check_sum = 0;
// ������㥬 䠩�
if(TextFile::scan(32768, 0, false) != -1)
{
    return check_sum;
}
return BAD_CHECKSUM;
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// ������ ����� 䠩��
/// \param [in] char *str 㪠��⥫� �� ���� 䠩��
/// \param [in] int32_t pos ������ ��ப� � 䠩�� (����������)
/// \param [in] int len ������⢮ ᨬ����� � �����
/// \return 0 � ��砥 �ᯥ�, -1 � ��砥 �訡��, �ॡ��饩 �४�饭�� ࠧ��� 䠩��
int CheckSum::parse(char *str, int32_t pos, int len)
{
if(str && len > 0)
{
    uint32_t word;
	off_t bytes;
	do
	{
		word = 0;
		bytes = sizeof(uint32_t);
		if(len < bytes)
			bytes = len % sizeof(uint32_t);
		memmove(&word, str, bytes);
		check_sum += word;
		len -= bytes;
		str += bytes;
	}
	while(len > 0);
}
else
	return -1;
return 0;
}
///////////////////////////////////////////////////////////////////////////////

