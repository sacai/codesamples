#if !defined(__TEXTFILE_H)
#define __TEXTFILE_H

/** \file
	\brief ����䥩� ��� ࠧ��� ⥪�⮢��� 䠩�� �� ��ப�, ࠧ������� ��ன CR-LF (0x0d-0x0a)
	
	������ ����� TextFile �।��⠢��� �㭪樨 ������, ������� � ࠧ��� ⥪�⮢�� 䠩��� �� ��ப�, � ������������
	��८�।����� � �ந������� ����� �㭪�� ࠧ��� ������ ��ப� TextFile::parse

*/


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "sys_base.h"

#pragma pack(push,1)

extern "C" {

/// ������ ����� TextFile �।��⠢��� �㭪樨 ������, ������� � ࠧ��� ⥪�⮢�� 䠩��� �� ��ப�, � ������������
/// ��८�।����� � �ந������� ����� �㭪�� ࠧ��� ������ ��ப� TextFile::parse
class TextFile : public virtual DRTS_Base_Object
{
    char *tbuf;	///< �����⥫� �� ����
protected:

    int flag;	///< ����
	uint32_t flength; ///< ࠧ��� 䠩��
public:
    char *filename;	///< ��� 䠩��
    int handle;		///< ���ਯ�� 䠩��

    /// ��������� �� 㬮�砭��
    TextFile();

    /// ���������
    /// \param [in] char *fname ��� 䠩��
    /// \param [in] int mode ०�� ������ 䠩��, �� 㬮�砭�� ⮫쪮 ��� �⥭��
    /// \param [in] int access ०�� ����㯠 � 䠩��, �᫨ �� ᮧ������, �� 㬮�砭�� ����� �����
    TextFile(char *fName, int mode=O_RDONLY, int access=0777);

    /// ��������
    ~TextFile()  {close();};
    
    /// ����⨥ 䠩��
    /// \param [in] char *fname ��� 䠩��
    /// \param [in] int mode ०�� ������ 䠩��, �� 㬮�砭�� ⮫쪮 ��� �⥭��
    /// \param [in] int access ०�� ����㯠 � 䠩��, �᫨ �� ᮧ������, �� 㬮�砭�� ����� �����
    /// \return ���ਯ�� ����⮣� 䠩��, ���� -1 � ��砥 �訡��
    int open(char *fName, int mode=O_RDONLY, int access=0777);

    /// �����⨥ 䠩��
    /// \param [in] int remfile 㤠���� �� 䠩� �� �����⨨
    void close(int remfile = 0);

    /// �����஢���� 䠩��
    /// \param [in] unsigned bufsize ࠧ��� ���� �� ᪠��஢����
    /// \param [in] int32_t fromwhere ������ 䠩��, � ���ன ������ ������� ᪠��஢����
	/// \param [in] bool split ࠧ������ (true) ��� ��� (false) 䠩� �� ��ப�, � ��᫥���� ��砥 ���⮥ �����筮� �⥭��
    /// \return 0, �᫨ ࠧ��� �����訫�� �ᯥ譮, -1 � ��砥 �訡�� ���� ����筮�� ���뢠��� ᪠��஢����
    int scan(unsigned bufsize=512, int32_t fromwhere=0L, bool split=true);

    /// �㭪�� ࠧ��� ��ப�. ��।������ � �ந������� �����.
    /// \param [in] char *str ��ப� ��� ࠧ���
    /// \param [in] int32_t pos ������ ��ப� � 䠩��
    /// \param [in] int len ����� ��ப�
    /// \return 0, �᫨ ࠧ��� �����訫�� �ᯥ譮, -1 � ��砥 �訡�� ���� ����室����� ����筮�� ���뢠��� ᪠��஢����
    virtual int parse(char *str, int32_t pos, int len) {return 0;};

};

};

#pragma pack(pop)

#endif