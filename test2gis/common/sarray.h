#if !defined (__SARRAY_H)
#define __SARRAY_H
/** \file
 \brief ��������� �������᪮�� ���ᨢ� ��ப
 
  ��������� ����� "���ᨢ ��ப",
  �।�����饣� �।�⢠ �࠭���� ���ᨢ� ��ப c
  ��⮬���᪨� 㢥��祭��� ࠧ��� ���ᨢ� ��
  �ॢ�襭�� ���������� �࠭���.

*/

#include "array_t.h"

#pragma pack(push,1)

extern "C" {

/// �������᪨� ���ᨢ ��ப
class stringArray : public array_t<char*>
{
public:

	/// ��������� �� 㬮�砭��
	/// \param ���
	stringArray() : array_t<char*>()
	 {set_invalid((char *)NULL);};

	/// ���������
	/// \param [in] size_t startsize ��室�� ࠧ��� ���ᨢ�
	/// \param [in] size_t delta ������⢮ ����⮢, �� ���஥ 㢥��稢����� ࠧ���
	/// ���ᨢ� � �����।������ ������ �� ���௠��� ᢮������� ���� (�� 㬮�砭��
	/// ��ࠢ�������� �������� ��室���� ࠧ���)
	stringArray(size_t startsize, size_t delta = 0) : array_t<char*>(startsize, delta)
	 {set_invalid((char *)NULL);};

	/// ��������� ����஢����
	/// \param [in] const s_array &src ��室�� ���ᨢ
	stringArray(const stringArray &src) : array_t<char*>(src) 
	{
		for(int i = 0; i < size(); i++)
		{
			char *item = get(i);
			set(item, i);
		}
	};

	/// ��������
	~stringArray() {flush();};
	
	/// �������� �����
	/// \param [in] char *src ��室��� ���祭�� �����
	/// \return ���� ����� ���ᨢ�
	/// \return invalid() � ��砥 �訡��
	/// (�ਧ��� �訡�� ������ ���� �����饭 � getError())
	char* make_item(char *src)
	{
		char *item = invalid();
		if(src)
			setError(DRTSStrdup(item, src));
		return(item);
	};
	
	/// ���⪠ �����
	/// \param [in] char* &item ��뫪� �� ��頥�� �����
	/// \return EOK (0) � ��砥 �ᯥ�
	/// \return ENOMEM (12) �� �訡�� �᢮�������� �����
	int clean_item(char* &item)
	{
		int result = EINVAL;
		if(item != invalid())
			DRTSFree(item);
		setError(result);
		return(result);
	};

	/// ���������� ����� � ����� ���ᨢ� � ���� ������ +=
	/// \param [in] char* c ������塞�� ���祭��
	/// \return ��뫪� �� ����� ��ꥪ�
	stringArray& operator+=(char* c)  {add(c); return *this;};
	stringArray& operator+=(const char* c)  {add((char *)c); return *this;};

	/// ���������� ����� � ����� ���ᨢ� � ���� ������ +
	/// \param [in] char* c ������塞�� ���祭��
	/// \return ��뫪� �� ����� ��ꥪ�
	stringArray& operator+(char* c)  {add(c); return *this;};
	stringArray& operator+(const char* c)  {add((char *)c); return *this;};

	/// �������� ��᫥����� ����� � ���� ������ +-
	/// \param ���
	/// \return ��뫪� �� ����� ��ꥪ�
	stringArray& operator--() { removelast(); return *this;};

	/// ����஢�� ���ᨢ�
	/// \param ���
	/// \return EOK � ��砥 �ᯥ�
	/// \return ESRCH, �᫨ ���ᨢ ����
	int sort(void);

	/// ������� ���� �����
	/// \param [in] char *item ���祭�� ��ப� ��� ���᪠
	/// \return ���祭�� �����, ���� NULL � ��砥 �訡��
	char* find(char *item);
	char* find(const char *item) {return(find((char *)item));};

	/// ������ ���� � ���ᨢ�
	/// \param [in] char *item �᪮��� ���祭��
	/// \return ���祭�� �����, ���� NULL � ��砥 �訡��
	char* bfind(char *item);
	char* bfind(const char *item) {return(bfind((char *)item));};

};

typedef stringArray sArray;

/// ��������� foreach ��� �������᪮�� ���ᨢ� ��ப sArray.
/// ����஫� �� ��ࠢ���⢮ ����� NULL ����������� �� �ணࠬ����.
/// \param [in, out] item - ����� ���ᨢ�, �ᯮ��㥬� � 横��
/// \param [in] arrp - 㪠��⥫� �� ��ꥪ�-���ᨢ
/// \param [in] iter - ����� ���ᨢ�, ������� �� �맮�� �����
#define for_each_sarray(item, arrp, iter) for_each_array_ptr(char *, (item), (arrp), (iter))

/// ��������� foreach ��� �������᪮�� ���ᨢ� ��ப sArray � ����஫�� �� ��ࠢ���⢮ ����� NULL.
/// \param [in, out] item - ����� ���ᨢ�, �ᯮ��㥬� � 横��
/// \param [in] arrp - 㪠��⥫� �� ��ꥪ�-���ᨢ
/// \param [in] iter - ����� ���ᨢ�, ������� �� �맮�� �����
#define for_each_sarray_null(item, arrp, iter) for_each_array_ptr_valid(char *, (item), (arrp), (iter))

};

#pragma pack(pop)

#endif