#include <locale.h>
#include "minicurs.h"

int menu_item_handler(char *str, uint32_t int_data, float float_data)
{
system(str);
return 1;
}

int main(int argc, char *argv[])
{
	mcurses_handle_t curses = INVALID_MCURSES_HANDLE;
	menu_handle_t menu = INVALID_MENU_HANDLE;
	int res = EINVAL;
	mcurses_menu_item_t menu_items[8] = 
	{{"File list", '1', "ls -l;sleep 5", 0, 0.},
	 {"Место на дисках", '2', "df -h;sleep 5 ", 0, 0.},
	 {"Список процессов", '3', "ps -A;sleep 5 ", 0, 0.},
	 {"Информация", '4', "pidin info;sleep 5 ", 0, 0.},
	 {"Информация", '5', "pidin hwinfo;sleep 5", 0, 0.},
	 {"Информация", '6', "pidin cpuinfo;sleep 5", 0, 0.},
	 {"Информация", '7', "pidin asinfo;sleep 5", 0, 0.},
	 {"Выход", '8', "EXITMENU", 0, 0.}};

	setlocale(LC_CTYPE, "C-TRADITIONAL");
	
	if(mcurses_init(&curses) != EOK)
		exit(1);
		
	if(mcurses_set_term(curses) != EOK)
		exit(1);

	res = mcurses_menu_init(&menu, curses, menu_item_handler, "Выберите пункт меню", 8, menu_items);
	if(res == EOK)
	{
		res = mcurses_menu_run(menu);
		if(res != EOK)
			printf("%s: %d\n", "Ошибка при отображении меню", res);
		mcurses_menu_close(&menu);
	}
	else
		printf("%s: %d\n", "Ошибка при инициализации меню", res);

	mcurses_restore_term(curses);
	mcurses_close(&curses);
	return(0);
}