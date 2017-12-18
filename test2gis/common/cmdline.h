#ifndef _CMDLINE_
#define _CMDLINE_

/** \file
  \brief �࣠������ ࠧ��� ��������� ��ப� POSIX-�ਫ������
  
  ����� 䠩� ᮤ�ন� ॠ������ ࠧ��� ��������� ��ப� POSIX-�ਫ������
*/

#include "appcmdl.h"
#include "textbase.h"

#pragma pack(push,1)

extern "C" {

///////////////////////////////////////////////////////////////////////////////
/// �࣠������ ࠧ��� ��������� ��ப� POSIX-�ਫ������               
class CommandLine : public AppCommandLine
{
    char **args;                     ///< �����⥫� �� ���ᨢ ��㬥�⮢
	int fromString;                  ///< ���ᨢ ��㬥�⮢ ��ନ஢�� �� ��ப�

public:

    /// ��������� �� 㬮�砭��
    /// \param ���
	CommandLine(void) {Init();};

    /// ��������� ��� ��ࠡ�⪨ �⠭���⭮� ��������� ��ப�
    /// \param [in] int cnt ������⢮ ��㬥�⮢ ��������� ��ப�
    /// \param [in] char **arg ���ᨢ ��㬥�⮢
    /// \param [in] unsigned char s ᯮᮡ �।�⠢����� ��㬥�⮢ � ��������� ��ப� (�� 㬮�砭�� UNIX_LIKE)
    CommandLine(int cnt, char **arg, unsigned char s = UNIX_LIKE) {Init(cnt, arg, s);};

    /// ��������� ��� ��ࠡ�⪨ ��������� ��ப� � �⨫� Windows (��㬥��� ����� ᯫ�譮� ��ப��)
    /// \param [in] char *string ��ப� ��㬥�⮢
    /// \param [in] unsigned char s ᯮᮡ �।�⠢����� ��㬥�⮢ � ��������� ��ப�
    CommandLine(char *string, unsigned char s = DOS_LIKE) {Init(string, s);};

	// ���樠������ �� 㬮�砭��
    /// \param ���
	/// \return TRUE (1)
	int Init(void) {args = NULL; fromString = FALSE; return(TRUE);};

    /// ���樠������ ��� ��ࠡ�⪨ �⠭���⭮� ��������� ��ப�
    /// \param [in] int cnt ������⢮ ��㬥�⮢ ��������� ��ப�
    /// \param [in] char **arg ���ᨢ ��㬥�⮢
    /// \param [in] unsigned char s ᯮᮡ �।�⠢����� ��㬥�⮢ � ��������� ��ப�
    /// \return 1 � ��砥 �ᯥ�, 0 � ��砥 �訡��
	int Init(int cnt, char **arg, unsigned char s = UNIX_LIKE);

    /// ���樠������ ��� ��ࠡ�⪨ ��������� ��ப� � �⨫� Windows (��㬥��� ����� ᯫ�譮� ��ப��)
    /// \param [in] char *string ��ப� ��㬥�⮢
    /// \param [in] unsigned char s ᯮᮡ �।�⠢����� ��㬥�⮢ � ��������� ��ப�
    /// \return 1 � ��砥 �ᯥ�, 0 � ��砥 �訡��
	int Init(char *string, unsigned char s = DOS_LIKE);

    /// ��������
    /// \param ���
    ~CommandLine() {Close();};

	/// �����⨥
    /// \param ���
    /// \return ���
	void Close(void);

    /// ���������� ��㬥�� � ���ᨢ ��㬥�⮢
    /// \param [in] char *argstr ��ப� ��㬥��
    /// \return 1 � ��砥 �ᯥ�, 0 � ��砥 �訡��
    int addArg(char *argstr);

    /// ������� ��।��� ��㬥�� ��������� ��ப�
    /// \param [in] int item - ���浪��� ����� ���㬥��
    /// \return /// 㪠��⥫� �� ��㬥�� ��� NULL � ��砥 ���ᯥ�
    virtual char *getArg(int item) {return(args != NULL && item < count ? args[item] : NULL);};

};

///////////////////////////////////////////////////////////////////////////////
/// �࣠������ ࠧ��� 䠩�� ��ࠬ��஢ ��������� ��ப�
/// �� ���� ��ꥪ� ��� ࠧ��� ���䨣��樮���� 䠩���               
class CommandFile : public TextBase
{
    CommandLine *cmd;                ///< �����⥫� �� ��������� ��ப�

public:

    /// ���������
    /// \param [in] char *name ��� 䠩��
    /// \param [in] int mode ०�� ������ 䠩��
    /// \param [in] int access ०�� ����㯠 � 䠩��
    CommandFile(char *name, int mode=O_RDONLY, int access=0777);

    /// ��������
    /// \param ���
    ~CommandFile();

    /// �����஢���� 䠩�� ��ࠬ��஢
    /// \param [in] int bufsize ࠧ��� ���� �� ᪠��஢���� � ࠧ��� 䠩��
    /// \param [in,out] CommandLine *cml ��ꥪ� ��������� ��ப�, � ������ ��������� ��ࠬ����
    /// \return 1 � ��砥 �ᯥ�, 0 � ��砥 �訡��
    int scan(int bufsize, CommandLine *cml);

    /// ������ ��ப� 䠩�� ��ࠬ��஢
    /// \param [in] char *str ��ப� 䠩�� ��ࠬ��஢
    /// \param [in] int32_t pos ������ ��ப� � 䠩��
    /// \param [in] int len ����� ��ப� � 䠩��
    /// \return 0 � ��砥 �ᯥ�, -1 � ��砥 �訡��, �ॡ��饩 �४�饭�� ࠧ��� 䠩��
    virtual int parse(char *str, int32_t pos, int len);

};

};

#pragma pack(pop)


#endif