#ifndef __MINI_CURSES__
#define __MINI_CURSES__

//////////////////////////////////////////////////////////////////////////////
/** \file
 	\brief ����䥩� ��� �뢮�� �� ���᮫� � �ᯮ�짮������ Esc-��᫥����⥫쭮�⥩ VT100
 	
 	����䥩� ��� �뢮�� �� ���᮫� � �ᯮ�짮������ Esc-��᫥����⥫쭮�⥩ VT100,
 	������ ncurses ��� ������� � ����饭��.
 */

#include <stddef.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#pragma pack(push,1)

#ifndef EZERO
#define EZERO 0		///< ��।������ �ਧ���� ������⢨� �訡�� (errno == 0)
#endif
#ifndef EOK
#define EOK EZERO	///< ��८�।������ �ਧ���� ������⢨� �訡�� EOK, �᫨ �� �� ��।���� � �⠭���⭮� ���������
#endif

#define ESC_BEGIN	"\033["		///< ��砫� �ந����쭮� ��᫥����⥫쭮�� Esc[
#define ESC_NB		"\033"		///< ��砫� �ந����쭮� ��᫥����⥫쭮�� Esc
#define ESC_SGR1	ESC_BEGIN "1m"	///< ���� ⥪��
#define ESC_SGR4	ESC_BEGIN "4m"	///< ����ભ��� ⥪��
#define ESC_SGR5	ESC_BEGIN "5m"	///< �����騩 ⥪��
#define ESC_SGR7	ESC_BEGIN "7m"	///< ������஢���� ⥪�� (��� �� �����)
#define ESC_SGR0	ESC_BEGIN "0m"	///< ������ ��������� ��ਡ�⮢ ⥪��
#define ESC_ED2	 	ESC_BEGIN "H" ESC_BEGIN "2J"	///< ���⪠ �࠭� � ��६�饭��� ����� � ���� ���孨� 㣮�
#define ESC_EL0 	ESC_BEGIN "K"	///< ���⪠ ⥪�饩 ��ப� �� ����� � �� ����
#define ESC_EL2 	ESC_BEGIN "2K"	///< ���⪠ ⥪�饩 ��ப� �� ��砫� � �� ����
#define ESC_HOM		ESC_BEGIN "H"	///< ��६�饭�� ����� � ���� ���孨� 㣮�
#define ESC_IND		ESC_NB "D"		///< ��६�饭�� ����� �� ���� ��ப� ���� � �ப��⪮�
#define ESC_RI		ESC_NB "M"		///< ��६�饭�� ����� �� ���� ��ப� ����� � �ப��⪮�
#define ESC_CPL		ESC_BEGIN "F"		///< ��६�饭�� ����� � ��砫� �।��饩 ��ப� � �ப��⪮�
#define ESC_CNL		ESC_BEGIN "E"		///< ��६�饭�� ����� � ��砫� ᫥���饩 ��ப� � �ப��⪮�
#define ESC_DECSC	ESC_NB "7"		///< ���࠭��� ������ � ��ࠬ���� �����
#define ESC_DECRC	ESC_NB "8"		///< ����⠭����� ������ � ��ࠬ���� �����
#define ESC_RIS		ESC_NB "c"		///< ������ �ନ��� � ��室���� ���ﭨ�
#define ESC_CUU		ESC_BEGIN "A"	///< ��६�饭�� ����� �� ���� ��ப� �����
#define ESC_CUD		ESC_BEGIN "B"	///< ��६�饭�� ����� �� ���� ��ப� ����
#define ESC_SD		ESC_BEGIN "S"		///< ��६�饭�� ����� �� ���� ��ப� ���� � �ப��⪮�
#define ESC_SU		ESC_BEGIN "T"		///< ��६�饭�� ����� �� ���� ��ப� ����� � �ப��⪮�
#define ESC_HIDE_CUR	ESC_BEGIN "?25l"		///< ����⨥ �����
#define ESC_SHOW_CUR	ESC_BEGIN "?25h"		///< ����⨥ �����


#define ESC_SYM	'\033'		///< ������ ��砫� Esc-��᫥����⥫쭮��
#define BRK_SYM	'['			///< ��ன ᨬ��� ��砫� Esc-��᫥����⥫쭮��
#define FIN_SYM	'~'			///< �������� �������騩 ᨬ��� Esc-��᫥����⥫쭮��
#define ENTER_SYM	'\n'	///< ������ �롮� �࠭���� ����� (<Enter>)
#define UP_SYM	'A'			///< ����� �����
#define DN_SYM	'B'			///< ����� ����
#define RT_SYM	'C'			///< ����� ��ࠢ�
#define LT_SYM	'D'			///< ����� �����
#define PGDN_SYM	'U'		///< ��࠭�� ����
#define PGUP_SYM	'V'		///< ��࠭�� �����
#define HOME_SYM	'H'		///< � ��砫�
#define END_SYM		'Y'		///< � �����
#define PGDN_DIG	'6'		///< ��࠭�� ���� (�᫮)
#define PGUP_DIG	'5'		///< ��࠭�� ����� (�᫮)
#define HOME_DIG	'1'		///< � ��砫� (�᫮)
#define END_DIG		'4'		///< � ����� (�᫮)
#define QUIT_SYM	'Q'		///< ������ ��室� �� �࠭���� ����� (Q)
#define qUIT_SYM	'q'		///< ������ ��室� �� �࠭���� ����� (q)

/// ��ਡ��� ⥪��
enum txt_attr
{
	ta_normal = 0,		///< ����� ⥪��
	ta_bold = 1,		///< ����
	ta_underline = 2,	///< ����ભ���
	ta_blink = 4,		///< �����騩
	ta_inv = 8			///< ������஢���� (��� �� �����)
};

/// ������� ��� �࠭���� �ᯮ����⥫쭮� ���ଠ樨
struct minicurses_t
{
	struct termios newt, oldt;	///< ��६���� ��� ���樠����樨 �ନ����, ��� �⥭�� �������� ᨬ�����
	int is_term_set;			///< ��⠭����� �� �ନ��� � raw
	int lines;					///< ������⢮ ��ப ���� �ନ����
	int columns;				///< ������⢮ ������� ��� �ନ����
	char term_type[80];			///< ��ப�-�����䨪��� ⨯� �ନ����
	int is_region_set;			///< ��⠭������ �� �������� �ப��⪨
};

/// ������� ���ᠭ�� ����⮩ ������
struct mcurses_key_t
{
	int code;			///< ��� ᨬ����
	int is_esc;			///< �� Esc-��᫥����⥫쭮���?
};

/// ��� �㭪樨-��ࠡ��稪� ����⨩ ������
/// \param [in] ptrdiff_t handle - ���ਯ�� �࠭���� �����
/// \param [in] mcurses_key_t *key - ���ᠭ�� ����⮩ ������
/// \return �㭪�� ������ �������� 1 ��� �த������� ࠡ��� �࠭���� �����, ���� 0 ��� ��室�
typedef int (*mcurses_key_handler_t)(ptrdiff_t handle, mcurses_key_t *key);

#define MAX_MENU_ITEM	72	///< ���ᨬ���� ࠧ��� �⮡ࠦ������ ����� ����� ����
#define MAX_MENU_STR	255	///< ���ᨬ���� ࠧ��� �࠭���� ��ப� ����� ����

#define MENU_CMD_EXIT	"EXITMENU"	///< ������� ��室� �� ����

/// ������� ����� ����
struct mcurses_menu_item_t
{
	char text[MAX_MENU_ITEM];		///< �⮡ࠦ����� ��� ����� ����
	char sym;						///< ������ ����ண� ����㯠
	struct
	{
		char str[MAX_MENU_STR];		///< �࠭���� ��ப�
		uint32_t int_data;			///< �����᫥��� �����
		float float_data;			///< ����� � ������饩 �窮�
	} item;							///< ����ন��� ����� ����
};

/// ��� �㭪樨-��ࠡ��稪�, ��뢠���� �� �롮� ���짮��⥫�� ����� ����
/// \param [in] char *str - �࠭���� ��ப� ����� ����
/// \param [in] uint32_t int_data - �࠭��� 楫��᫥��� ����� ����� ����
/// \param [in] float float_data - �࠭��� ����� � ������饩 �窮� ����� ����
/// \return �㭪�� ������ �������� 1 ��� �த������� ࠡ��� ����, ���� 0 ��� ��室�
typedef int (*mcurses_menu_item_handler_t)(char *str, uint32_t int_data, float float_data);

/// ������� ���ᠭ�� ����
struct mcurses_menu_t
{
	minicurses_t *mcurses;			///< �����⥫� �� ���ᠭ�� ���੥� �뢮�� �� ���᮫�
	char header[MAX_MENU_ITEM];		///< ��������� ����
	size_t count;					///< ������⢮ ����⮢ ����
	size_t allocated;				///< ������⢮ ����⮢, ��� ���஥ �뤥���� ������
	int selected;					///< ��࠭�� �㭪� ����
	int width;						///< ��ਭ� ����
	int first_line, last_line, cur_line;		///< ��ࢠ�, ��᫥����, ⥪��� ��ப� ���� ����
	int first_item, last_item;		///< ���� � ��᫥���� �⮡ࠦ���� ������ ���� 
	mcurses_key_handler_t key_func;	///< �㭪��-��ࠡ��稪 ������ ������
	mcurses_menu_item_handler_t item_func;	///< �㭪��-��ࠡ��稪 ����� ����
	mcurses_menu_item_t *items;		///< ���ᨢ ����⮢ ����
};

/// ��� ���ਯ�� ����䥩� �뢮�� �� ���᮫�
typedef ptrdiff_t	mcurses_handle_t;

/// ����୮� ���祭�� ���ਯ�� ����䥩�
const mcurses_handle_t INVALID_MCURSES_HANDLE=(mcurses_handle_t)-1;

/// ��� ���ਯ�� ����
typedef ptrdiff_t	menu_handle_t;

/// ����୮� ���祭�� ���ਯ�� ����
const menu_handle_t INVALID_MENU_HANDLE=(menu_handle_t)-1;

extern "C"
{

/// ���樠������ ����䥩� �뢮�� �� ���᮫�
/// \param [in, out] minicurses_t *mcurses - 㪠��⥫� �� �������� ���ᠭ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return EINVAL, �᫨ 㪠��⥫� �� ���樠����஢��
int minicurses_init(minicurses_t *mcurses);

/// ����ன�� �ନ���� ��� ���뢠��� ����⨩ ������
/// \param [in] minicurses_t *mcurses - 㪠��⥫� �� �������� ���ᠭ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return EINVAL, �᫨ 㪠��⥫� �� ���樠����஢�� ��� �ନ��� 㦥 ����஥�
/// \return �訡�� ����ன�� �ନ����
int minicurses_set_term(minicurses_t *mcurses);

/// ����⠭������� ����஥� �ନ���� �� �����襭�� ���뢠��� ����⨩ ������
/// \param [in] minicurses_t *mcurses - 㪠��⥫� �� �������� ���ᠭ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return EINVAL, �᫨ 㪠��⥫� �� ���樠����஢�� ��� �ନ��� �� �� ����஥�
/// \return �訡�� ����ன�� �ନ����
int minicurses_restore_term(minicurses_t *mcurses);

/// ���樠������ ����䥩� �뢮�� �� ���᮫� � ᮧ������ ���ਯ��
/// \param [in, out] mcurses_handle_t *handle - 㪠��⥫� �� ���ਯ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return EINVAL, �᫨ 㪠��⥫� �� ���樠����஢��
/// \return �ਧ��� �訡��
int mcurses_init(mcurses_handle_t *handle);

/// �����⨥ ����䥩� �뢮�� �� ���᮫� � ᮧ������ ���ਯ��
/// \param [in, out] mcurses_handle_t handle - 㪠��⥫� �� ���ਯ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return EINVAL, �᫨ 㪠��⥫� �� ���樠����஢��
/// \return �ਧ��� �訡��
int mcurses_close(mcurses_handle_t *handle);

/// ����ன�� �ନ���� ��� ���뢠��� ����⨩ ������
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
inline int mcurses_set_term(mcurses_handle_t handle) {if(handle == INVALID_MCURSES_HANDLE) return EINVAL; minicurses_t *mcurses = (minicurses_t *)handle; return (minicurses_set_term(mcurses));};

/// ����⠭������� ����஥� �ନ���� �� �����襭�� ���뢠��� ����⨩ ������
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
inline int mcurses_restore_term(mcurses_handle_t handle) {if(handle == INVALID_MCURSES_HANDLE) return EINVAL; minicurses_t *mcurses = (minicurses_t *)handle; return (minicurses_restore_term(mcurses));};

/// ����祭�� ⥪�饣� ������⢠ ��ப �� �࠭�
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \param [out] int *lines - 㪠��⥫� �� ��६����� ��� ������⢮ ��ப 
/// \return EOK � ��砥 �ᯥ�
/// \return EINVAL, �᫨ 㪠��⥫� �� ���樠����஢��
inline int mcurses_get_lines(mcurses_handle_t handle, int *lines) {if(handle == INVALID_MCURSES_HANDLE || !lines) return EINVAL; minicurses_t *mcurses = (minicurses_t *)handle; *lines = mcurses->lines; return EOK;};

/// ����祭�� ⥪�饣� ������⢠ ������� �� �࠭�
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \param [out] int *columns - 㪠��⥫� �� ��६����� ��� ������⢮ ��ப 
/// \return EOK � ��砥 �ᯥ�
/// \return EINVAL, �᫨ 㪠��⥫� �� ���樠����஢��
inline int mcurses_get_columns(mcurses_handle_t handle, int *columns) {if(handle == INVALID_MCURSES_HANDLE || !columns) return EINVAL; minicurses_t *mcurses = (minicurses_t *)handle; *columns = mcurses->columns; return EOK;};

/// ���⪠ �࠭�
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
inline int mcurses_cls(mcurses_handle_t handle) {int res = printf("%s%s", ESC_ED2, ESC_HOM); if(res > 0) return EOK; return errno;};

/// ���⪠ ��ப�
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
inline int mcurses_clstr(mcurses_handle_t handle) {int res = printf("%s", ESC_EL2); if(res > 0) return EOK; return errno;};

/// ��⠭����� �࠭��� �ப��⪨ �࠭�
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \param [in] int from - ����� ��ࢮ� ��ப�, ��稭�� � 1
/// \param [in] int to - ����� ��᫥���� ��ப�, ��稭�� � 1, ����� 祬 from+1
/// \return EOK � ��砥 �ᯥ�
/// \return EINVAL � ��砥 ������� ��ࠬ��஢
/// \return �訡�� �뢮�� �� ���᮫�
inline int mcurses_set_region(mcurses_handle_t handle, int from, int to)
 {if(handle == INVALID_MCURSES_HANDLE) return EINVAL; minicurses_t *mcurses = (minicurses_t *)handle;
  if(from > 0 && to > 0 && to > from + 2) {int res = printf("%s%d;%d%s", ESC_BEGIN, from, to, "r"); if(res > 0) {mcurses->is_region_set = 1; return EOK;}; return errno;}; return EINVAL;};

/// ������ �࠭��� �ப��⪨ �࠭�
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
inline int mcurses_reset_region(mcurses_handle_t handle)
 {if(handle <= 0) return EINVAL; minicurses_t *mcurses = (minicurses_t *)handle;
  if(mcurses->is_region_set) {int res = printf("%s%d;%d%s", ESC_BEGIN, 1, mcurses->lines, "r"); if(res > 0) {mcurses->is_region_set = 0; return EOK;}; return errno;}; return EOK;};

/// ������ �ନ��� � ��室���� ���ﭨ�
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
inline int mcurses_reset_term(mcurses_handle_t handle) {int res = printf("%s", ESC_RIS); if(res > 0) return EOK; return errno;};

/// ��६����� ����� � ��砫� ᫥���饩 ��ப� � �ப��⪮�
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
inline int mcurses_move_next_line(mcurses_handle_t handle) {int res = printf("%s", ESC_CNL); if(res > 0) return EOK; return errno;};

/// ��६����� ����� � ��砫� �।��饩 ��ப� � �ப��⪮�
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
inline int mcurses_move_prev_line(mcurses_handle_t handle) {int res = printf("%s", ESC_CPL); if(res > 0) return EOK; return errno;};

/// ��६����� ����� � 㪠������ ������
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \param [in] int line - ����� ��ப�, �� 1 �� 24
/// \param [in] int col - ����� �������, �� 1 �� 80
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
inline int mcurses_set_cursor_to(mcurses_handle_t handle, int line, int col)
 {if(line > 0 && col > 0) {int res = printf("%s%d;%d%s", ESC_BEGIN, line, col, "H"); if(res > 0) return EOK; return errno;}; return EINVAL;};

/// ��६����� ����� � 㪠������ ������� � ⥪�饩 ��ப�
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \param [in] int col - ����� �������, �� 1 �� 80
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
inline int mcurses_set_cursor_to_col(mcurses_handle_t handle, int col)
 {if(col > 0) {int res = printf("%s%d`", ESC_BEGIN, col); if(res > 0) return EOK; return errno;}; return EINVAL;};

/// ������� ⥪���� ������ �����
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \param [out] int *line - 㪠��⥫� �� ���� ��� ����� ��ப�
/// \param [out] int *col - 㪠��⥫� �� ���� ��� ����� �������
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
int mcurses_get_cursor_pos(mcurses_handle_t handle, int *line, int *col);

/// ��६����� ����� �� ᫥������ ��ப� � �ப��⪮�
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
inline int mcurses_move_down(mcurses_handle_t handle) {int res = printf("%s", ESC_CUD); if(res > 0) return EOK; return errno;};

/// ��६����� ����� �� �।����� ��ப� � �ப��⪮�
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
inline int mcurses_move_up(mcurses_handle_t handle) {int res = printf("%s", ESC_CUU); if(res > 0) return EOK; return errno;};

/// �ப����� ᯨ᮪ ����
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \param [in] int lines - ������⢮ ��ப (�� 㬮�砭�� 1)
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
inline int mcurses_scroll_down(mcurses_handle_t handle, int lines = 1) {int res = printf("%s%d%s", ESC_BEGIN, lines, "S"); if(res > 0) return EOK; return errno;};

/// �ப����� ᯨ᮪ �����
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \param [in] int lines - ������⢮ ��ப (�� 㬮�砭�� 1)
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
inline int mcurses_scroll_up(mcurses_handle_t handle, int lines = 1) {int res = printf("%s%d%s", ESC_BEGIN, lines, "T"); if(res > 0) return EOK; return errno;};

/// ���࠭��� ������ � ��ࠬ���� �����
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
inline int mcurses_save_cursor(mcurses_handle_t handle) {int res = printf("%s", ESC_DECSC); if(res > 0) return EOK; return errno;};

/// ����⠭����� ������ � ��ࠬ���� �����
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
inline int mcurses_restore_cursor(mcurses_handle_t handle) {int res = printf("%s", ESC_DECRC); if(res > 0) return EOK; return errno;};

/// ������ �����
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
inline int mcurses_hide_cursor(mcurses_handle_t handle) {int res = printf("%s", ESC_HIDE_CUR); if(res > 0) return EOK; return errno;};

/// �������� �����
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
inline int mcurses_show_cursor(mcurses_handle_t handle) {int res = printf("%s", ESC_SHOW_CUR); if(res > 0) return EOK; return errno;};

/// �뢥�� ⥪�� �� ���᮫� � 㪠����묨 ��ਡ�⠬�
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \param [in] const char *text - ⥪�⮢�� ��ப� ��� �뢮��
/// \param [in] int attr - ��ਡ��� ⥪�� (�� 㬮�砭�� ����� ⥪��)
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
int mcurses_put_text(mcurses_handle_t handle, const char *text, int attr = ta_normal);

/// �뢥�� ��ப� �� ���᮫� � 㪠����묨 ��ਡ�⠬�,
/// � �।���⥫쭮� ���⪮� ��ப�
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \param [in] const char *text - ⥪�⮢�� ��ப� ��� �뢮��
/// \param [in] int attr - ��ਡ��� ⥪�� (�� 㬮�砭�� ����� ⥪��)
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
int mcurses_put_string(mcurses_handle_t handle, const char *text, int attr = ta_normal);
 
/// �⠭���⭠� �㭪��-��ࠡ��稪 ����⨩ ������ � ����
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����
/// \param [in] mcurses_key_t *key - ���ᠭ�� ����⮩ ������
/// \return 1 ��� �த������� ࠡ��� �࠭���� �����, ���� 0 ��� ��室�
inline int mcurses_menu_key_handler(mcurses_handle_t handle, mcurses_key_t *key) {return 1;};

/// ���樠������ �࠭���� ���� � ᮧ������ ���ਯ��
/// \param [in, out] menu_handle_t *handle - 㪠��⥫� �� ���ਯ�� ����
/// \param [in] mcurses_handle_t mcurses - ���ਯ�� ����䥩�
/// \param [in] mcurses_menu_item_handler_t item_func - �㭪��-��ࠡ��稪 �㭪� ����
/// \param [in] const char *header - ��������� ����
/// \param [in] size_t items_count - ������⢮ ����⮢ ���� (�� 㬮�砭�� 0)
/// \param [in] mcurses_menu_item_t *items - ���ᨢ ����⮢ ���� (�� 㬮�砭�� �� ���樠����஢��)
/// \param [in] mcurses_key_handler_t key_func - �㭪��-��ࠡ��稪 ����⨩ ������ (�� 㬮�砭�� - �⠭�����)
/// \return EOK � ��砥 �ᯥ�
/// \return EINVAL, �᫨ 㪠��⥫� �� ���樠����஢���
/// \return �ਧ��� �訡��
int mcurses_menu_init(menu_handle_t *handle, mcurses_handle_t mcurses, mcurses_menu_item_handler_t item_func, const char *header, size_t items_count = 0,
                      mcurses_menu_item_t *items = NULL,
                      mcurses_key_handler_t key_func = mcurses_menu_key_handler);

/// �����⨥ �࠭���� ����
/// \param [in, out] menu_handle_t *handle - 㪠��⥫� �� ���ਯ�� ����
/// \return EOK � ��砥 �ᯥ�
/// \return EINVAL, �᫨ 㪠��⥫� �� ���樠����஢���
/// \return �ਧ��� �訡��
int mcurses_menu_close(menu_handle_t *handle);

/// ���������� �㭪� ����
/// \param [in] menu_handle_t handle - ���ਯ�� ����
/// \param [in] const char *text - �⮡ࠦ���� ⥪��
/// \param [in] char sym - ᨬ��� ����ண� ����㯠 (0 - �⪫�砥� ������ �����)
/// \param [in] const char *str - �࠭���� ��ப�
/// \param [in] uint32_t int_data - �࠭��� 楫��᫥��� �����
/// \param [in] float float_data - �࠭��� ����� � ������饩 �窮�
/// \return EOK � ��砥 �ᯥ�
/// \return EINVAL, �᫨ 㪠��⥫� �� ���樠����஢���
/// \return �ਧ��� �訡��
int mcurses_menu_add(menu_handle_t handle, const char *text, char sym, const char *str, uint32_t int_data, float float_data);

/// ����� � �㭪�� ���� �� ���浪����� ������
/// \param [in] menu_handle_t handle - ���ਯ�� ����
/// \param [in] int item - ���浪��� ����� �㭪�
/// \return 㪠��⥫� �� �㭪� ����
/// \return NULL � ��砥 �訡��
mcurses_menu_item_t *mcurses_menu_item(menu_handle_t handle, int item);

/// ����� � �㭪�� ���� �� �⮡ࠦ������ ⥪���
/// \param [in] menu_handle_t handle - ���ਯ�� ����
/// \param [in] const char *text - �⮡ࠦ���� ⥪��
/// \return 㪠��⥫� �� �㭪� ����
/// \return NULL � ��砥 �訡��
mcurses_menu_item_t *mcurses_menu_by_text(menu_handle_t handle, const char *text);

/// ����� � �㭪�� ���� �� ᨬ���� ����ண� ����㯠
/// \param [in] menu_handle_t handle - ���ਯ�� ����
/// \param [in] char sym - ᨬ��� ����ண� ����㯠
/// \return 㪠��⥫� �� �㭪� ����
/// \return NULL � ��砥 �訡��
mcurses_menu_item_t *mcurses_menu_by_sym(menu_handle_t handle, char sym);

/// �⮡ࠦ���� � ����� ����
/// \param [in] menu_handle_t handle - ���ਯ�� ����
/// \return EOK � ��砥 �ᯥ�
/// \return �ਧ��� �訡��
int mcurses_menu_run(menu_handle_t handle);

/// �⮡ࠦ���� ����
/// \param [in] menu_handle_t handle - ���ਯ�� ����
/// \return EOK � ��砥 �ᯥ�
/// \return �ਧ��� �訡��
int mcurses_menu_display(menu_handle_t handle);

/// �⮡ࠦ���� �㭪� ����
/// \param [in] menu_handle_t handle - ���ਯ�� ����
/// \param [in] int item - ���浪��� ����� �㭪�
/// \param [in] int selected - �뤥���� (1) ��� ��� (0) �㭪�
/// \return EOK � ��砥 �ᯥ�
/// \return �ਧ��� �訡��
int mcurses_menu_item_display(menu_handle_t handle, int item, int selected);

/// ��।������ �ਭ� ����
/// \param [in] menu_handle_t handle - ���ਯ�� ����
/// \return EOK � ��砥 �ᯥ�
/// \return �ਧ��� �訡��
int mcurses_menu_define_width(menu_handle_t handle);


};

#pragma pack(pop)

#endif