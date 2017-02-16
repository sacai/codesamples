//////////////////////////////////////////////////////////////////////////////
/** \file
 	\brief ����䥩� ��� �뢮�� �� ���᮫� � �ᯮ�짮������ Esc-��᫥����⥫쭮�⥩ VT100
 	
 	����䥩� ��� �뢮�� �� ���᮫� � �ᯮ�짮������ Esc-��᫥����⥫쭮�⥩ VT100,
 	������ ncurses ��� ������� � ����饭��.
 */

#include <sys/ioctl.h>
#include "minicurs.h"

///////////////////////////////////////////////////////////////////////////////
/// ���樠������ ����䥩� �뢮�� �� ���᮫�
/// \param [in, out] minicurses_t *mcurses - 㪠��⥫� �� �������� ���ᠭ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return EINVAL, �᫨ 㪠��⥫� �� ���樠����஢��
int minicurses_init(minicurses_t *mcurses)
{
int result = EINVAL;
char *env_ptr = NULL;

if(mcurses)
{
	memset(mcurses, 0, sizeof(minicurses_t));
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	mcurses->lines = w.ws_row;
	mcurses->columns = w.ws_col;
	env_ptr = getenv("TERM");
	if(env_ptr)
		strncpy(mcurses->term_type, env_ptr, sizeof(mcurses->term_type) - 1);
	result = EOK;
}

return(result);
};
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// ����ன�� �ନ���� ��� ���뢠��� ����⨩ ������
/// \param [in] minicurses_t *mcurses - 㪠��⥫� �� �������� ���ᠭ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return EINVAL, �᫨ 㪠��⥫� �� ���樠����஢�� ��� �ନ��� 㦥 ����஥�
/// \return �訡�� ����ன�� �ନ����
int minicurses_set_term(minicurses_t *mcurses)
{
int result = EINVAL;

if(mcurses)
{
	if(!mcurses->is_term_set)
	{
		if(tcgetattr(STDIN_FILENO, &mcurses->oldt) == 0)
		{
			memmove(&mcurses->newt, &mcurses->oldt, sizeof(termios));
			mcurses->newt.c_lflag &= ~(ICANON | ECHO);
			if(tcsetattr(STDIN_FILENO, TCSANOW, &mcurses->newt) == 0)
			{
				mcurses->is_term_set = 1;
				result = EOK;
				return(result);
			}
		}
		result = errno;
	}
}

return(result);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// ����⠭������� ����஥� �ନ���� �� �����襭�� ���뢠��� ����⨩ ������
/// \param [in] minicurses_t *mcurses - 㪠��⥫� �� �������� ���ᠭ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return EINVAL, �᫨ 㪠��⥫� �� ���樠����஢�� ��� �ନ��� �� �� ����஥�
/// \return �訡�� ����ன�� �ନ����
int minicurses_restore_term(minicurses_t *mcurses)
{
int result = EINVAL;

if(mcurses)
{
	if(mcurses->is_term_set)
	{
 		if(tcsetattr(STDIN_FILENO, TCSANOW, &mcurses->oldt) == 0)
 		{
			mcurses->is_term_set = 1;
			result = EOK;
			return(result);
 		}
		result = errno;
 	}
}

return(result);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// ���樠������ ����䥩� �뢮�� �� ���᮫� � ᮧ������ ���ਯ��
/// \param [in, out] mcurses_handle_t *handle - 㪠��⥫� �� ���ਯ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return EINVAL, �᫨ 㪠��⥫� �� ���樠����஢��
/// \return �ਧ��� �訡��
int mcurses_init(mcurses_handle_t *handle)
{
int result = EINVAL;
minicurses_t *mcurses = NULL;

if(handle)
{
	mcurses = (minicurses_t *)malloc(sizeof(minicurses_t));
	if(mcurses)
	{
		*handle = (mcurses_handle_t)mcurses;
		result = minicurses_init(mcurses);
	}
	else
	{
		*handle = INVALID_MCURSES_HANDLE;
		result = errno;
	}
}

return(result);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// �����⨥ ����䥩� �뢮�� �� ���᮫� � ᮧ������ ���ਯ��
/// \param [in, out] mcurses_handle_t handle - 㪠��⥫� �� ���ਯ�� ����䥩�
/// \return EOK � ��砥 �ᯥ�
/// \return EINVAL, �᫨ 㪠��⥫� �� ���樠����஢��
int mcurses_close(mcurses_handle_t *handle)
{
int result = EINVAL;
minicurses_t *mcurses = NULL;

if(handle && handle > 0)
{
	mcurses = (minicurses_t *)*handle;
	free(mcurses);
	*handle = INVALID_MCURSES_HANDLE;
	result = EOK;
}

return(result);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// ������� ⥪���� ������ �����
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \param [out] int *line - 㪠��⥫� �� ���� ��� ����� ��ப�
/// \param [out] int *col - 㪠��⥫� �� ���� ��� ����� �������
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
int mcurses_get_cursor_pos(mcurses_handle_t handle, int *line, int *col)
{
int result = EINVAL;
int res_p = -1;
int sym;
char coord[MAX_MENU_STR], *ptr = coord;

if(line && col)
{
	res_p = printf("%s%d%s", ESC_BEGIN, 6, "n");
	if(res_p > 0)
	{
		sym = getchar();
		if(sym == ESC_SYM)
		{
			sym = getchar();
			if(sym == BRK_SYM)
			{
				memset(coord, 0, sizeof(coord));
				ptr = coord;
				do
				{
					sym = getchar();
					if(sym == ';')
					{
						*line = atoi(coord);
						memset(coord, 0, sizeof(coord));
						ptr = coord;
					}
					else if(sym == 'R')
					{
						*col = atoi(coord);
						break;
					}
					else
					{
						*ptr = sym;
						ptr++;
					}
				} while(sym != EOF);
				result = EOK;
			}
		}
	}
	else
		result = errno;
}

return(result);
}
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// �ᯮ����⥫쭠� �㭪�� ����ன�� ����஢ ᨬ����� ��� �뢮�� �� ���᮫�
void SetGTables()
{
printf("\x0F");              // Select GL to G0
printf("%c%c", ESC_SYM, 0x7E);        // Select GR to G1
printf("%c%c%c", ESC_SYM, 0x28, 0x42);  // Select G0 to ASCII charset.
printf("%c%c%c", ESC_SYM, 0x29, 0x3C);  // Select G1 to supplement charset.
printf("%c%c%c", ESC_SYM, 0x2A, 0x55);  // Select G2 to PCterm.
printf("%s", ESC_BEGIN "11m");
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// �ᯮ����⥫쭠� �㭪�� ����⠭������� ����஢ ᨬ����� ��᫥ �뢮�� �� ���᮫�
void RestoreGTables()
{
printf("%s", ESC_BEGIN "10m");
printf("%c%c%c", ESC_SYM, 0x2A, 0x3C);  // Select G2 to PCterm.
printf("%c%c%c", ESC_SYM, 0x29, 0x30);  // Select G1 to special charset.
printf("%c%c%c", ESC_SYM, 0x28, 0x42);  // Select G0 to ASCII charset.
printf("%c%c%c", ESC_SYM, 0x7E);        // Select GR to G1
printf("\x0F");              // Select GL to G0
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// �뢥�� ⥪�� �� ���᮫� � 㪠����묨 ��ਡ�⠬�
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \param [in] const char *text - ⥪�⮢�� ��ப� ��� �뢮��
/// \param [in] int attr - ��ਡ��� ⥪�� (�� 㬮�砭�� ����� ⥪��)
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
int mcurses_put_text(mcurses_handle_t handle, const char *text, int attr)
{
int result = EINVAL;
int res_p = -1;

if(text && strlen(text))
{
	result = EOK;
	res_p = printf("%s", ESC_SGR0);
	if(res_p > 0 && (attr & ta_bold))
	{
		res_p = printf("%s", ESC_SGR1);
	}
	if(res_p > 0 && (attr & ta_underline))
	{
		res_p = printf("%s", ESC_SGR4);
	}
	if(res_p > 0 && (attr & ta_blink))
	{
		res_p = printf("%s", ESC_SGR5);
	}
	if(res_p > 0 && (attr & ta_inv))
	{
		res_p = printf("%s", ESC_SGR7);
	}
	if(res_p > 0)
	{
		//SetGTables();
		res_p = printf("%s", text);
		//RestoreGTables();
	}
	if(res_p >= 0 && attr)
	{
		res_p = printf("%s", ESC_SGR0);
	}
	if(res_p < 0)
		result = errno;
}

return(result);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// �뢥�� ��ப� �� ���᮫� � 㪠����묨 ��ਡ�⠬�,
/// � �।���⥫쭮� ���⪮� ��ப�
/// \param [in] mcurses_handle_t handle - ���ਯ�� ����䥩�
/// \param [in] const char *text - ⥪�⮢�� ��ப� ��� �뢮��
/// \param [in] int attr - ��ਡ��� ⥪�� (�� 㬮�砭�� ����� ⥪��)
/// \return EOK � ��砥 �ᯥ�
/// \return �訡�� �뢮�� �� ���᮫�
int mcurses_put_string(mcurses_handle_t handle, const char *text, int attr)
{
int result = EINVAL;
int res_p = -1;

if(text && strlen(text))
{
	res_p = printf("%s", ESC_EL0);
	if(res_p > 0)
	{
		result = mcurses_put_text(handle, text, attr);
		if(result == EOK)
			res_p = printf("%s", ESC_EL0);
	}
	if(res_p < 0)
		result = errno;
}

return(result);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
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
int mcurses_menu_init(menu_handle_t *handle, mcurses_handle_t mcurses, mcurses_menu_item_handler_t item_func, const char *header, size_t items_count,
                      mcurses_menu_item_t *items,
                      mcurses_key_handler_t key_func)
{
int result = EINVAL;
mcurses_menu_t *menu = NULL;

if(handle && mcurses > 0)
{
	menu = (mcurses_menu_t *)malloc(sizeof(mcurses_menu_t));
	if(menu)
	{
		memset(menu, 0, sizeof(mcurses_menu_t));
		menu->mcurses = (minicurses_t *)mcurses;
		if(header)
		{
			strncpy(menu->header, header, sizeof(menu->header)-2);
			strcat(menu->header, ":");
		}
		menu->item_func = item_func;
		menu->key_func = key_func;
		if(items_count)
		{
			menu->allocated = items_count;
			menu->items = (mcurses_menu_item_t *)malloc(sizeof(mcurses_menu_item_t) * menu->allocated);
			if(menu->items)
			{
				memset(menu->items, 0, sizeof(mcurses_menu_item_t) * menu->allocated);
				if(items)
				{
					memmove(menu->items, items, sizeof(mcurses_menu_item_t) * menu->allocated);
					menu->count = items_count;
					mcurses_menu_define_width((menu_handle_t)menu);
				}
				result = EOK;
				*handle = (menu_handle_t)menu;
			}
			else
			{
				result = errno;
				free(menu);
				*handle = INVALID_MENU_HANDLE;
			}
		}
		else
		{
			result = EOK;
			*handle = (menu_handle_t)menu;
		}
	}
	else
	{
		*handle = INVALID_MENU_HANDLE;
		result = errno;
	}
	
}

return(result);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// �����⨥ �࠭���� ����
/// \param [in, out] menu_handle_t *handle - 㪠��⥫� �� ���ਯ�� ����
/// \return EOK � ��砥 �ᯥ�
/// \return EINVAL, �᫨ 㪠��⥫� �� ���樠����஢���
/// \return �ਧ��� �訡��
int mcurses_menu_close(menu_handle_t *handle)
{
int result = EINVAL;
mcurses_menu_t *menu = NULL;

if(handle && *handle > 0)
{
	menu = (mcurses_menu_t *)*handle;
	if(menu->items)
	{
		free(menu->items);
		menu->items = NULL;
	}
	free(menu);
	*handle = INVALID_MENU_HANDLE;
	result = EOK;
}

return(result);
}
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
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
int mcurses_menu_add(menu_handle_t handle, const char *text, char sym, const char *str, uint32_t int_data, float float_data)
{
int result = EINVAL;
mcurses_menu_t *menu = NULL;
int width = 0;

if(handle > 0 && text && strlen(text))
{
	menu = (mcurses_menu_t *)handle;
	if(menu->allocated == 0)
	{
		menu->allocated = 1;
		menu->items = (mcurses_menu_item_t *)malloc(sizeof(mcurses_menu_item_t) * menu->allocated);
	}
	else if(menu->count >= menu->allocated)
	{
		menu->allocated *= 2;
		menu->items = (mcurses_menu_item_t *)realloc(menu->items, sizeof(mcurses_menu_item_t) * menu->allocated);
	}
	if(menu->items)
	{
		memset(&menu->items[menu->count], 0, sizeof(mcurses_menu_item_t));
		strncpy(menu->items[menu->count].text, text, sizeof(menu->items[menu->count].text)-1);
		if((sym >= 'A' && sym <= 'Z') || (sym >= '0' && sym <= '9'))
			menu->items[menu->count].sym = sym;
		if(str && strlen(str))
			strcpy(menu->items[menu->count].item.str, str);
		menu->items[menu->count].item.int_data = int_data;
		menu->items[menu->count].item.float_data = float_data;
		width = strlen(menu->items[menu->count].text);
		if(width > menu->width)
			menu->width = width;
		if(menu->width > menu->mcurses->columns)
			menu->width = menu->mcurses->columns;
		menu->count++;
		result = EOK;
	}
	else
		result = errno;
}

return(result);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// ����� � �㭪�� ���� �� ���浪����� ������
/// \param [in] menu_handle_t handle - ���ਯ�� ����
/// \param [in] int item - ���浪��� ����� �㭪�
/// \return 㪠��⥫� �� �㭪� ����
/// \return NULL � ��砥 �訡��
mcurses_menu_item_t *mcurses_menu_item(menu_handle_t handle, int item)
{
mcurses_menu_item_t *result = NULL;
mcurses_menu_t *menu = NULL;

if(handle > 0)
{
	menu = (mcurses_menu_t *)handle;
	if(item < menu->count)
		result = &menu->items[item];
}

return(result);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// ����� � �㭪�� ���� �� �⮡ࠦ������ ⥪���
/// \param [in] menu_handle_t handle - ���ਯ�� ����
/// \param [in] const char *text - �⮡ࠦ���� ⥪��
/// \return 㪠��⥫� �� �㭪� ����
/// \return NULL � ��砥 �訡��
mcurses_menu_item_t *mcurses_menu_by_text(menu_handle_t handle, const char *text)
{
mcurses_menu_item_t *result = NULL;
mcurses_menu_t *menu = NULL;
int item;

if(handle > 0 && text && strlen(text))
{
	menu = (mcurses_menu_t *)handle;
	for(item = 0; item < menu->count; item++)
	{
		if(!strcmp(menu->items[item].text, text))
		{
			result = &menu->items[item];
			break;
		}
	}
}

return(result);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// ����� � �㭪�� ���� �� ᨬ���� ����ண� ����㯠
/// \param [in] menu_handle_t handle - ���ਯ�� ����
/// \param [in] char sym - ᨬ��� ����ண� ����㯠
/// \return 㪠��⥫� �� �㭪� ����
/// \return NULL � ��砥 �訡��
mcurses_menu_item_t *mcurses_menu_by_sym(menu_handle_t handle, char sym)
{
mcurses_menu_item_t *result = NULL;
mcurses_menu_t *menu = NULL;
int item;

if(handle > 0)
{
	menu = (mcurses_menu_t *)handle;
	for(item = 0; item < menu->count; item++)
	{
		if(menu->items[item].sym == sym)
			result = &menu->items[item];
		else if(sym >= 'a' && sym <= 'z' && menu->items[item].sym == sym - 32)
			result = &menu->items[item];
		if(result)
			break;
	}
}

return(result);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// �⮡ࠦ���� � ����� ����
/// \param [in] menu_handle_t handle - ���ਯ�� ����
/// \return EOK � ��砥 �ᯥ�
/// \return �ਧ��� �訡��
int mcurses_menu_run(menu_handle_t handle)
{
int result = EINVAL;
mcurses_menu_t *menu = NULL;
mcurses_menu_item_t *item = NULL;
int sym = 0, i = 0;
mcurses_key_t key;
int stop = 0;

if(handle > 0)
{
	menu = (mcurses_menu_t *)handle;
	result = mcurses_menu_display(handle);
	if(result == EOK)
	{
		//mcurses_hide_cursor((uint32_t)menu->mcurses);
		while(!stop)
		{
			item = NULL;
			key.code = 0;
			key.is_esc = 0;
			sym = getchar();
			if(sym == EOF)
				stop = 1;
			else if(sym == ESC_SYM)
			{
				key.is_esc = 1;
				alarm(1);
				sym = getchar();
				alarm(0);
				if(sym == EOF)
					stop = 1;
				else if(sym == BRK_SYM)
				{
					sym = getchar();
					key.code = sym;
					if(isdigit(key.code))
					{
						sym = getchar();
						if(sym == EOF)
							stop = 1;
						else if(sym == FIN_SYM)
						{
							switch(key.code)
							{
								case PGUP_DIG:
									key.code = PGUP_SYM;
								break;
								case HOME_DIG:
									key.code = HOME_SYM;
								break;
								case PGDN_DIG:
									key.code = PGDN_SYM;
								break;
								case END_DIG:
									key.code = END_SYM;
								break;
							}							
						}
					}
					switch(key.code)
					{
						case UP_SYM:
							if(menu->selected > 0)
							{
								mcurses_menu_item_display(handle, menu->selected, 0);
								menu->cur_line--;
								menu->selected--;
								if(menu->cur_line >= menu->first_line)
									mcurses_move_up((mcurses_handle_t)menu->mcurses);
								else
								{
									menu->cur_line = menu->first_line;
									mcurses_scroll_up((mcurses_handle_t)menu->mcurses);
									menu->first_item--;
									menu->last_item--;
								}
								mcurses_menu_item_display(handle, menu->selected, 1);
							}
							else
							{
								menu->selected = menu->count - 1;
								mcurses_menu_display(handle);
							}
						break;
						case DN_SYM:
							if(menu->selected < menu->count - 1)
							{
								mcurses_menu_item_display(handle, menu->selected, 0);
								menu->cur_line++;
								menu->selected++;
								if(menu->cur_line <= menu->last_line)
									mcurses_move_down((mcurses_handle_t)menu->mcurses);
								else
								{
									menu->cur_line = menu->last_line;
									mcurses_scroll_down((mcurses_handle_t)menu->mcurses);
									menu->first_item++;
									menu->last_item++;
								}
								mcurses_menu_item_display(handle, menu->selected, 1);
							}
							else
							{
								menu->selected = 0;
								mcurses_menu_display(handle);
							}
						break;
						case PGUP_SYM:
							mcurses_menu_item_display(handle, menu->selected, 0);
							menu->selected = menu->first_item;
							mcurses_set_cursor_to((mcurses_handle_t)menu->mcurses, menu->first_line, 1);
							menu->cur_line = menu->first_line;
							mcurses_menu_item_display(handle, menu->selected, 1);
						break;
						case HOME_SYM:
							mcurses_menu_item_display(handle, menu->selected, 0);
							menu->selected = 0;
							mcurses_set_cursor_to((mcurses_handle_t)menu->mcurses, menu->first_line, 1);
							menu->cur_line = menu->first_line;
							if(menu->first_item > 0)
							{
								i = menu->first_item;
								while(i >= menu->selected)
								{
									mcurses_scroll_up((mcurses_handle_t)menu->mcurses);
									mcurses_menu_item_display(handle, i, (i == menu->selected));
									if(i > menu->selected)
									{
										menu->first_item--;
										menu->last_item--;
									}
									i--;
								}
							}
							else
								mcurses_menu_item_display(handle, menu->selected, 1);
						break;
						case PGDN_SYM:
							mcurses_menu_item_display(handle, menu->selected, 0);
							menu->selected = menu->last_item;
							mcurses_set_cursor_to((mcurses_handle_t)menu->mcurses, menu->last_line, 1);
							menu->cur_line = menu->last_line;
							mcurses_menu_item_display(handle, menu->selected, 1);
						break;
						case END_SYM:
							mcurses_menu_item_display(handle, menu->selected, 0);
							menu->selected = menu->count - 1;
							mcurses_set_cursor_to((mcurses_handle_t)menu->mcurses, menu->last_line, 1);
							menu->cur_line = menu->last_line;
							if(menu->last_item < menu->count - 1)
							{
								i = menu->last_item;
								while(i <= menu->selected)
								{
									mcurses_scroll_down((mcurses_handle_t)menu->mcurses);
									mcurses_menu_item_display(handle, i, (i == menu->selected));
									if(i < menu->selected)
									{
										menu->first_item++;
										menu->last_item++;
									}
									i++;
								}
							}
							else
								mcurses_menu_item_display(handle, menu->selected, 1);
						break;
						default:
							stop = !menu->key_func(handle, &key);
						break;
					}
				}
				else
				{
					key.code = sym;
					stop = !menu->key_func(handle, &key);
				}
			}
			else if(sym == QUIT_SYM || sym == qUIT_SYM)
			{
				stop = 1;
			}
			else
			{
				if(sym == ENTER_SYM)
					item = mcurses_menu_item(handle, menu->selected);
				else if(isalpha(sym) || isdigit(sym))
					item = mcurses_menu_by_sym(handle, sym);
				if(item)
				{
					mcurses_reset_region((mcurses_handle_t)menu->mcurses);
					mcurses_cls((mcurses_handle_t)menu->mcurses);
					mcurses_put_string((mcurses_handle_t)menu->mcurses, (char *)"\n", ta_normal);
					//mcurses_show_cursor((uint32_t)menu->mcurses);
					if(!strcmp(item->item.str, MENU_CMD_EXIT))
						stop = 1;
					else
					{
						stop = !menu->item_func(item->item.str, item->item.int_data, item->item.float_data);
						if(!stop)
						{
							mcurses_menu_display(handle);
							//mcurses_hide_cursor((uint32_t)menu->mcurses);
						}
					}
				}
				else
				{
					key.code = sym;
					stop = !menu->key_func(handle, &key);
				}
			}
		}
	}
	mcurses_reset_region((mcurses_handle_t)menu->mcurses);
	mcurses_cls((mcurses_handle_t)menu->mcurses);
	mcurses_put_string((mcurses_handle_t)menu->mcurses, (char *)"\n", ta_normal);
//	mcurses_show_cursor((uint32_t)menu->mcurses);
}

return(result);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// �⮡ࠦ���� ����
/// \param [in] menu_handle_t handle - ���ਯ�� ����
/// \return EOK � ��砥 �ᯥ�
/// \return �ਧ��� �訡��
int mcurses_menu_display(menu_handle_t handle)
{
int result = EINVAL;
mcurses_menu_t *menu = NULL;
int i = 0, current, attr = ta_normal, set_reg = 0;
char str[MAX_MENU_STR], tpl[MAX_MENU_STR];

if(handle > 0)
{
	menu = (mcurses_menu_t *)handle;
	if(menu->count && menu->items)
	{
		mcurses_reset_region((mcurses_handle_t)menu->mcurses);
		mcurses_cls((mcurses_handle_t)menu->mcurses);
		memset(str, 0, sizeof(str));
		memset(tpl, 0, sizeof(tpl));
		snprintf(tpl, sizeof(str) - 1, " %s%d%s ", "%", -menu->width, "s");
		snprintf(str, sizeof(str) - 1, tpl, menu->header);
		mcurses_set_cursor_to((mcurses_handle_t)menu->mcurses, 1, 1);
		mcurses_put_string((mcurses_handle_t)menu->mcurses, str);
		menu->first_line = menu->cur_line = 2;
		menu->last_line = menu->first_line + menu->count - 1;
		if(menu->last_line > menu->mcurses->lines - 1)
		{
			menu->last_line = menu->mcurses->lines - 1;
			set_reg = 1;
		}
		menu->first_item = 0;
		for(i = 0; i < menu->count; i++)
		{
			if(menu->cur_line <= menu->last_line)
			{
				mcurses_menu_item_display(handle, i, (menu->selected == i));
				menu->last_item = i;
				if(menu->selected == i)
					current = menu->cur_line;
				menu->cur_line++;
			}
		}
		if(set_reg)
			mcurses_set_region((mcurses_handle_t)menu->mcurses, menu->first_line, menu->last_line);
		if(menu->selected > menu->last_item)
		{
			mcurses_set_cursor_to((mcurses_handle_t)menu->mcurses, menu->last_line, 1);
			menu->cur_line = menu->last_line;
			i = menu->last_item;
			while(i <= menu->selected)
			{
				mcurses_scroll_down((mcurses_handle_t)menu->mcurses);
				mcurses_menu_item_display(handle, i, (menu->selected == i));
				if(i < menu->selected)
				{
					menu->first_item++;
					menu->last_item++;
				}
				i++;
			}
		}
		else
		{
			mcurses_set_cursor_to((mcurses_handle_t)menu->mcurses, current, 1);
			menu->cur_line = current;
		}
		result = EOK;
	}
}

return(result);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// �⮡ࠦ���� �㭪� ����
/// \param [in] menu_handle_t handle - ���ਯ�� ����
/// \param [in] int item - ���浪��� ����� �㭪�
/// \param [in] int selected - �뤥���� (1) ��� ��� (0) �㭪�
/// \return EOK � ��砥 �ᯥ�
/// \return �ਧ��� �訡��
int mcurses_menu_item_display(menu_handle_t handle, int item, int selected)
{
int result = EINVAL, res_get = EINVAL;
mcurses_menu_t *menu = NULL;
int attr = ta_normal;
char str[MAX_MENU_STR], tpl[MAX_MENU_STR];
int line = 1, col = 1;

if(handle > 0)
{
	menu = (mcurses_menu_t *)handle;
	if(menu->count && menu->items && item < menu->count)
	{
		attr = (selected ? ta_inv : ta_normal);
		memset(str, 0, sizeof(str));
		mcurses_set_cursor_to((mcurses_handle_t)menu->mcurses, menu->cur_line, 1);
		mcurses_clstr((mcurses_handle_t)menu->mcurses);
		if(menu->items[item].sym)
		{
			snprintf(str, sizeof(str) - 1, " %c", menu->items[item].sym);
			mcurses_put_text((mcurses_handle_t)menu->mcurses, str, ta_bold);
			mcurses_put_text((mcurses_handle_t)menu->mcurses, (char *)":", ta_normal);
		}
		memset(str, 0, sizeof(str));
		memset(tpl, 0, sizeof(tpl));
		snprintf(tpl, sizeof(str) - 1, " %s%d%s ", "%", -menu->width, "s");
		snprintf(str, sizeof(str) - 1, tpl, menu->items[item].text);
		mcurses_put_text((mcurses_handle_t)menu->mcurses, str, attr);
		mcurses_set_cursor_to((mcurses_handle_t)menu->mcurses, menu->cur_line, 1);
	}
}

return(result);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// ��।������ �ਭ� ����
/// \param [in] menu_handle_t handle - ���ਯ�� ����
/// \return EOK � ��砥 �ᯥ�
/// \return �ਧ��� �訡��
int mcurses_menu_define_width(menu_handle_t handle)
{
int result = EINVAL;
mcurses_menu_t *menu = NULL;
int i = 0, width = 0;
char str[MAX_MENU_STR];

if(handle > 0)
{
	menu = (mcurses_menu_t *)handle;
	if(menu->count && menu->items)
	{
		for(i = 0; i < menu->count; i++)
		{
			width = strlen(menu->items[i].text);
			if(width > menu->width)
				menu->width = width;
		}
		if(menu->width > menu->mcurses->columns)
			menu->width = menu->mcurses->columns;
		result = EOK;
	}
}

return(result);
}
///////////////////////////////////////////////////////////////////////////////

