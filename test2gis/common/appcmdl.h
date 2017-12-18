#ifndef _APPCMDLINE_
#define _APPCMDLINE_

/** \file
 \brief ������ ����� ��� ࠧ��� ��������� ��ப�
 
 � ������ 䠩�� ����뢠���� ������ �����, ॠ�����騩 �㭪樨 ࠧ��� ��������� ��ப�. ��������� ���
 �������� ������� ������ ���� ��।����� � �ந������� �����.

*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "sys_base.h"
#include "sarray.h"

#pragma pack(push,1)

// ����⠭��, ��।����騥 ���譥� �।�⠢����� ��㬥�⮢

#define DASH_PREFIX       0x01           ///< ��䨪� ���祢��� ᫮�� - ᨬ��� '-'
#define SLASH_PREFIX      0x02           ///< ��䨪� ���祢��� ᫮�� - ᨬ��� '/'

/// ���⢨⥫쭮��� � ॣ����� �㪢
#define CASE_SENS         0x04

/// ��������� ��ப� ������ ���� �।�⠢���� � DOS-�⨫�
#define DOS_LIKE          DASH_PREFIX | SLASH_PREFIX

/// ��������� ��ப� ������ ���� �।�⠢���� � UNIX-�⨫�
#define UNIX_LIKE         DASH_PREFIX | CASE_SENS

/// �������, ����� ����� �⤥���� ���祢�� ᫮�� �� ��ࠬ���
#define CMD_DIV           ":="

extern "C" {


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ������ ����� ��� ࠧ��� ��������� ��ப�
/// \details � 楫�� ��⨬���樨 ����� ���� �⪫�祭 ࠧ��� ��㬥�⮢ � ������饩
/// ����⮩ �� ����� ������樨 _NO_FLOAT_ARGS � ����� ���������
class AppCommandLine : public DRTS_Base_Object
{
protected:

    int count;                       ///< ���稪 ��㬥�⮢
    unsigned char style;             ///< ���譥� �।�⠢����� ��㬥�⮢
    int current;                     ///< ������ ������ � ᯨ᪥ ��㬥�⮢
	sArray opts;                     ///< ���᮪ �����⨬�� ��権

    /// �஢�ઠ ��।���� ��㬥�� �� ����稥 ���祢��� ᫮��
    /// \param [in] char *key - ���祢�� ᫮��
    /// \param [in] char **arg - ���ᨢ ��㬥�⮢ ��������� ��ப�
    /// \return
    /// 1, �᫨ ���祢�� ᫮�� �������, � ��⨢��� ��砥 0
    int checkKey(char *key, char **arg);

    /// ������� ��।��� ��㬥�� ��������� ��ப�. ��८�।������ � �ந������� �����.
    /// \param [in] int item - ���浪��� ����� ���㬥��
    /// \return �㭪�� ������ �������� 㪠��⥫� �� ��㬥�� ��� NULL � ��砥 ���ᯥ�
    virtual char *getArg(int item) {return NULL;};

    /// �����஢��� �������� ������⢮ ᨬ����� ��ப� �� ��� ��� ��ॢ��� ��ப�
    /// \param [out] char *dst - ����-�ਥ����
    /// \param [in] const char *src - ��ப�-���筨�
    /// \param size_t n - ������⢮ �����㥬�� ᨬ�����
    /// \return 㪠��⥫� �� ����-�ਥ���� ��� NULL � ��砥 ���ᯥ�
    char *strncpyEx(char *dst, const char *src, size_t n);

public:

    /// ���������
	/// \param ���
    AppCommandLine() {opts.init(20); count = 0; style = 0; current = 0;};

    /// ��������
	/// \param ���
    ~AppCommandLine() {opts.flush(); count = 0; style = 0; current = 0;};

    /// ������� ���祭�� ��ࠬ��� �� ���祢��� ᫮��
    /// � ᮮ⢥��⢨� � ������� �ଠ⮬
    /// \param [in] const char *key - ���祢�� ᫮�� ��� ᨬ���
    /// \param [in] int next - ���� ������� �� ��砫� ��������� ��ப� (0) ���� �� ��᫥����� ���������� ��㬥�� (1)
    /// \param [in] const char *format - ��ப� �ଠ� ��� ����뢠��� ��㬥��
    /// \param [out] void *param - 㪠��⥫� �� ���� ��� ���祭�� ��㬥��
    /// \return ������ ���������� ��㬥�� � ���������� ��ப�, ���� -1 � ��砥 �訡��
    int getParameter(const char *key, int next, const char *format, void *param);

    /// ������� ���祭�� ��ப����� ��ࠬ���
    /// \param [in] const char *key - ���祢�� ᫮�� ��� ᨬ���
    /// \param [in] int next - ���� ������� �� ��砫� ��������� ��ப� (0) ���� �� ��᫥����� ���������� ��㬥�� (1)
    /// \param [out] char *param - 㪠��⥫� �� ��ப�-�ਥ���� ���祭�� ��㬥��
    /// \param [in] unsigned size - ࠧ��� ��ப�-�ਥ�����
    /// \return ������ ���������� ��㬥�� � ���������� ��ப�, ���� -1 � ��砥 �訡��
    int getStringParameter(const char *key, int next, char *param, unsigned size);

    /// �஢���� ����稥 ��ࠬ���-䫠�� (��� ���祭��)
    /// \param [in] const char *key - ���祢�� ᫮�� ��� ᨬ���
    /// \param [in] int next - ���� ������� �� ��砫� ��������� ��ப� (0) ���� �� ��᫥����� ���������� ��㬥�� (1)
    /// \param [out] int *param - 㪠��⥫� �� ��६�����, �ਭ������� ���祭�� 1, �᫨ ��ࠬ��� ������, � 0 � ��⨢��� ��砥
    /// \return ������ ���������� ��㬥�� � ���������� ��ப�, ���� -1 � ��砥 �訡��
    int getFlagParameter(const char *key, int next, int *param);

    /// ��⠭����� ������⢮ ��ࠬ��஢
    /// \param [in] int cnt - ������⢮ ��ࠬ��஢
    /// \return ���
    void setCount(int cnt) {count = cnt;};

    /// ������� ������⢮ ��ࠬ��஢
    /// \param ���
    /// \return ������⢮ ��ࠬ��஢
    int getCount(void) {return(count);};

    /// ��⠭����� ���譥� �।�⠢����� ��㬥�⮢
    /// \param [in] unsigned char s - ��᪠ ���譥�� �।�⠢����� ��㬥�⮢
    /// \return ���
    void setStyle(unsigned char s) {style = s;};

    /// ��⠭����� ����� �����⨬�� ��権. �᫨ ����� �� ��⠭�����, �� ����������.
    /// \param [in] const char *opt - ��ப� ���祩 � ���祢�� ᫮�, ࠧ�������� ������ﬨ
    /// \return ���
    void setOpt(const char *opt);

};

};

#pragma pack(pop)

#endif