#ifndef __MINI_CURSES__
#define __MINI_CURSES__

//////////////////////////////////////////////////////////////////////////////
/** \file
 	\brief Интерфейс для вывода на консоль с использованием Esc-последовательностей VT100
 	
 	Интерфейс для вывода на консоль с использованием Esc-последовательностей VT100,
 	аналог ncurses без излишеств и извращений.
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
#define EZERO 0		///< Определение признака отсутствия ошибки (errno == 0)
#endif
#ifndef EOK
#define EOK EZERO	///< Переопределение признака отсутствия ошибки EOK, если он не определен в стандартном заголовке
#endif

#define ESC_BEGIN	"\033["		///< Начало произвольной последовательности Esc[
#define ESC_NB		"\033"		///< Начало произвольной последовательности Esc
#define ESC_SGR1	ESC_BEGIN "1m"	///< Жирный текст
#define ESC_SGR4	ESC_BEGIN "4m"	///< Подчеркнутый текст
#define ESC_SGR5	ESC_BEGIN "5m"	///< Мерцающий текст
#define ESC_SGR7	ESC_BEGIN "7m"	///< Инвертированный текст (черный на белом)
#define ESC_SGR0	ESC_BEGIN "0m"	///< Очистить изменения атрибутов текста
#define ESC_ED2	 	ESC_BEGIN "H" ESC_BEGIN "2J"	///< Очистка экрана с перемещением курсора в левый верхний угол
#define ESC_EL0 	ESC_BEGIN "K"	///< Очистка текущей строки от курсора и до конца
#define ESC_EL2 	ESC_BEGIN "2K"	///< Очистка текущей строки от начала и до конца
#define ESC_HOM		ESC_BEGIN "H"	///< Перемещение курсора в левый верхний угол
#define ESC_IND		ESC_NB "D"		///< Перемещение курсора на одну строку вниз с прокруткой
#define ESC_RI		ESC_NB "M"		///< Перемещение курсора на одну строку вверх с прокруткой
#define ESC_CPL		ESC_BEGIN "F"		///< Перемещение курсора в начало предыдущей строки с прокруткой
#define ESC_CNL		ESC_BEGIN "E"		///< Перемещение курсора в начало следующей строки с прокруткой
#define ESC_DECSC	ESC_NB "7"		///< Сохранить позицию и параметры курсора
#define ESC_DECRC	ESC_NB "8"		///< Восстановить позицию и параметры курсора
#define ESC_RIS		ESC_NB "c"		///< Сбросить терминал к исходному состоянию
#define ESC_CUU		ESC_BEGIN "A"	///< Перемещение курсора на одну строку вверх
#define ESC_CUD		ESC_BEGIN "B"	///< Перемещение курсора на одну строку вниз
#define ESC_SD		ESC_BEGIN "S"		///< Перемещение курсора на одну строку вниз с прокруткой
#define ESC_SU		ESC_BEGIN "T"		///< Перемещение курсора на одну строку вверх с прокруткой
#define ESC_HIDE_CUR	ESC_BEGIN "?25l"		///< Скрытие курсора
#define ESC_SHOW_CUR	ESC_BEGIN "?25h"		///< Скрытие курсора


#define ESC_SYM	'\033'		///< Символ начала Esc-последовательности
#define BRK_SYM	'['			///< Второй символ начала Esc-последовательности
#define FIN_SYM	'~'			///< Возможный завершающий символ Esc-последовательности
#define ENTER_SYM	'\n'	///< Символ выбора экранного элемента (<Enter>)
#define UP_SYM	'A'			///< Курсор вверх
#define DN_SYM	'B'			///< Курсор вниз
#define RT_SYM	'C'			///< Курсор вправо
#define LT_SYM	'D'			///< Курсор влево
#define PGDN_SYM	'U'		///< Страница вниз
#define PGUP_SYM	'V'		///< Страница вверх
#define HOME_SYM	'H'		///< В начало
#define END_SYM		'Y'		///< В конец
#define PGDN_DIG	'6'		///< Страница вниз (число)
#define PGUP_DIG	'5'		///< Страница вверх (число)
#define HOME_DIG	'1'		///< В начало (число)
#define END_DIG		'4'		///< В конец (число)
#define QUIT_SYM	'Q'		///< Символ выхода из экранного элемента (Q)
#define qUIT_SYM	'q'		///< Символ выхода из экранного элемента (q)

/// Атрибуты текста
enum txt_attr
{
	ta_normal = 0,		///< Обычный текст
	ta_bold = 1,		///< Жирный
	ta_underline = 2,	///< Подчеркнутый
	ta_blink = 4,		///< Мерцающий
	ta_inv = 8			///< Инвертированный (черный на белом)
};

/// Структура для хранения вспомогательной информации
struct minicurses_t
{
	struct termios newt, oldt;	///< Переменные для инициализации терминала, для чтения вводимых символов
	int is_term_set;			///< Установлен ли терминал в raw
	int lines;					///< Количество строк окна терминала
	int columns;				///< Количество колонок она терминала
	char term_type[80];			///< Строка-идентификатор типа терминала
	int is_region_set;			///< Установлена ли областть прокрутки
};

/// Структура описания нажатой клавиши
struct mcurses_key_t
{
	int code;			///< Код символа
	int is_esc;			///< Это Esc-последовательность?
};

/// Тип функции-обработчика нажатий клавиш
/// \param [in] ptrdiff_t handle - дескриптор экранного элемента
/// \param [in] mcurses_key_t *key - описание нажатой клавиши
/// \return Функция должна возвращать 1 для продолжения работы экранного элемента, либо 0 для выхода
typedef int (*mcurses_key_handler_t)(ptrdiff_t handle, mcurses_key_t *key);

#define MAX_MENU_ITEM	72	///< Максимальный размер отображаемого имени элемента меню
#define MAX_MENU_STR	255	///< Максимальный размер хранимой строки элемента меню

#define MENU_CMD_EXIT	"EXITMENU"	///< Команда выхода из меню

/// Структура элемента меню
struct mcurses_menu_item_t
{
	char text[MAX_MENU_ITEM];		///< Отображаемое имя элемента меню
	char sym;						///< Символ быстрого доступа
	struct
	{
		char str[MAX_MENU_STR];		///< Хранимая строка
		uint32_t int_data;			///< Целочисленные данные
		float float_data;			///< Данные с плавающей точкой
	} item;							///< Содержимое элемента меню
};

/// Тип функции-обработчика, вызываемой при выборе пользователем элемента меню
/// \param [in] char *str - хранимая строка элемента меню
/// \param [in] uint32_t int_data - хранимые целочисленные данные элемента меню
/// \param [in] float float_data - хранимые данные с плавающей точкой элемента меню
/// \return Функция должна возвращать 1 для продолжения работы меню, либо 0 для выхода
typedef int (*mcurses_menu_item_handler_t)(char *str, uint32_t int_data, float float_data);

/// Структура описания меню
struct mcurses_menu_t
{
	minicurses_t *mcurses;			///< Указатель на описание интерйеса вывода на консоль
	char header[MAX_MENU_ITEM];		///< Заголовок меню
	size_t count;					///< Количество элементов меню
	size_t allocated;				///< Количество элементов, под которое выделена память
	int selected;					///< Выбранный пункт меню
	int width;						///< Ширина меню
	int first_line, last_line, cur_line;		///< Первая, последняя, текущая строки окна меню
	int first_item, last_item;		///< Первый и последний отображаемые элементы меню 
	mcurses_key_handler_t key_func;	///< Функция-обработчик нажатия клавиш
	mcurses_menu_item_handler_t item_func;	///< Функция-обработчик элемента меню
	mcurses_menu_item_t *items;		///< Массив элементов меню
};

/// Тип дескриптора интерфейса вывода на консоль
typedef ptrdiff_t	mcurses_handle_t;

/// Неверное значение дескриптора интерфейса
const mcurses_handle_t INVALID_MCURSES_HANDLE=(mcurses_handle_t)-1;

/// Тип дескриптора меню
typedef ptrdiff_t	menu_handle_t;

/// Неверное значение дескриптора меню
const menu_handle_t INVALID_MENU_HANDLE=(menu_handle_t)-1;

extern "C"
{

/// Инициализация интерфейса вывода на консоль
/// \param [in, out] minicurses_t *mcurses - указатель на структуру описания интерфейса
/// \return EOK в случае успеха
/// \return EINVAL, если указатель не инициализирован
int minicurses_init(minicurses_t *mcurses);

/// Настройка терминала для считывания нажатий клавиш
/// \param [in] minicurses_t *mcurses - указатель на структуру описания интерфейса
/// \return EOK в случае успеха
/// \return EINVAL, если указатель не инициализирован или терминал уже настроен
/// \return ошибка настройки терминала
int minicurses_set_term(minicurses_t *mcurses);

/// Восстановление настроек терминала при завершении считывания нажатий клавиш
/// \param [in] minicurses_t *mcurses - указатель на структуру описания интерфейса
/// \return EOK в случае успеха
/// \return EINVAL, если указатель не инициализирован или терминал не был настроен
/// \return ошибка настройки терминала
int minicurses_restore_term(minicurses_t *mcurses);

/// Инициализация интерфейса вывода на консоль с созданием дескриптора
/// \param [in, out] mcurses_handle_t *handle - указатель на дескриптор интерфейса
/// \return EOK в случае успеха
/// \return EINVAL, если указатель не инициализирован
/// \return признак ошибки
int mcurses_init(mcurses_handle_t *handle);

/// Закрытие интерфейса вывода на консоль с созданием дескриптора
/// \param [in, out] mcurses_handle_t handle - указатель на дескриптор интерфейса
/// \return EOK в случае успеха
/// \return EINVAL, если указатель не инициализирован
/// \return признак ошибки
int mcurses_close(mcurses_handle_t *handle);

/// Настройка терминала для считывания нажатий клавиш
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \return EOK в случае успеха
inline int mcurses_set_term(mcurses_handle_t handle) {if(handle == INVALID_MCURSES_HANDLE) return EINVAL; minicurses_t *mcurses = (minicurses_t *)handle; return (minicurses_set_term(mcurses));};

/// Восстановление настроек терминала при завершении считывания нажатий клавиш
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \return EOK в случае успеха
inline int mcurses_restore_term(mcurses_handle_t handle) {if(handle == INVALID_MCURSES_HANDLE) return EINVAL; minicurses_t *mcurses = (minicurses_t *)handle; return (minicurses_restore_term(mcurses));};

/// Получение текущего количества строк на экране
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \param [out] int *lines - указатель на переменную под количество строк 
/// \return EOK в случае успеха
/// \return EINVAL, если указатель не инициализирован
inline int mcurses_get_lines(mcurses_handle_t handle, int *lines) {if(handle == INVALID_MCURSES_HANDLE || !lines) return EINVAL; minicurses_t *mcurses = (minicurses_t *)handle; *lines = mcurses->lines; return EOK;};

/// Получение текущего количества колонок на экране
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \param [out] int *columns - указатель на переменную под количество строк 
/// \return EOK в случае успеха
/// \return EINVAL, если указатель не инициализирован
inline int mcurses_get_columns(mcurses_handle_t handle, int *columns) {if(handle == INVALID_MCURSES_HANDLE || !columns) return EINVAL; minicurses_t *mcurses = (minicurses_t *)handle; *columns = mcurses->columns; return EOK;};

/// Очистка экрана
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \return EOK в случае успеха
/// \return ошибку вывода на консоль
inline int mcurses_cls(mcurses_handle_t handle) {int res = printf("%s%s", ESC_ED2, ESC_HOM); if(res > 0) return EOK; return errno;};

/// Очистка строки
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \return EOK в случае успеха
/// \return ошибку вывода на консоль
inline int mcurses_clstr(mcurses_handle_t handle) {int res = printf("%s", ESC_EL2); if(res > 0) return EOK; return errno;};

/// Установить границы прокрутки экрана
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \param [in] int from - номер первой строки, начиная с 1
/// \param [in] int to - номер последней строки, начиная с 1, больше чем from+1
/// \return EOK в случае успеха
/// \return EINVAL в случае неверных параметров
/// \return ошибку вывода на консоль
inline int mcurses_set_region(mcurses_handle_t handle, int from, int to)
 {if(handle == INVALID_MCURSES_HANDLE) return EINVAL; minicurses_t *mcurses = (minicurses_t *)handle;
  if(from > 0 && to > 0 && to > from + 2) {int res = printf("%s%d;%d%s", ESC_BEGIN, from, to, "r"); if(res > 0) {mcurses->is_region_set = 1; return EOK;}; return errno;}; return EINVAL;};

/// Сбросить границы прокрутки экрана
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \return EOK в случае успеха
/// \return ошибку вывода на консоль
inline int mcurses_reset_region(mcurses_handle_t handle)
 {if(handle <= 0) return EINVAL; minicurses_t *mcurses = (minicurses_t *)handle;
  if(mcurses->is_region_set) {int res = printf("%s%d;%d%s", ESC_BEGIN, 1, mcurses->lines, "r"); if(res > 0) {mcurses->is_region_set = 0; return EOK;}; return errno;}; return EOK;};

/// Сбросить терминал к исходному состоянию
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \return EOK в случае успеха
/// \return ошибку вывода на консоль
inline int mcurses_reset_term(mcurses_handle_t handle) {int res = printf("%s", ESC_RIS); if(res > 0) return EOK; return errno;};

/// Переместить курсор в начало следующей строки с прокруткой
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \return EOK в случае успеха
/// \return ошибку вывода на консоль
inline int mcurses_move_next_line(mcurses_handle_t handle) {int res = printf("%s", ESC_CNL); if(res > 0) return EOK; return errno;};

/// Переместить курсор в начало предыдущей строки с прокруткой
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \return EOK в случае успеха
/// \return ошибку вывода на консоль
inline int mcurses_move_prev_line(mcurses_handle_t handle) {int res = printf("%s", ESC_CPL); if(res > 0) return EOK; return errno;};

/// Переместить курсор в указанную позицию
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \param [in] int line - номер строки, от 1 до 24
/// \param [in] int col - номер колонки, от 1 до 80
/// \return EOK в случае успеха
/// \return ошибку вывода на консоль
inline int mcurses_set_cursor_to(mcurses_handle_t handle, int line, int col)
 {if(line > 0 && col > 0) {int res = printf("%s%d;%d%s", ESC_BEGIN, line, col, "H"); if(res > 0) return EOK; return errno;}; return EINVAL;};

/// Переместить курсор в указанную колонку в текущей строке
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \param [in] int col - номер колонки, от 1 до 80
/// \return EOK в случае успеха
/// \return ошибку вывода на консоль
inline int mcurses_set_cursor_to_col(mcurses_handle_t handle, int col)
 {if(col > 0) {int res = printf("%s%d`", ESC_BEGIN, col); if(res > 0) return EOK; return errno;}; return EINVAL;};

/// Получить текущую позицию курсора
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \param [out] int *line - указатель на буфер под номер строки
/// \param [out] int *col - указатель на буфер под номер колонки
/// \return EOK в случае успеха
/// \return ошибку вывода на консоль
int mcurses_get_cursor_pos(mcurses_handle_t handle, int *line, int *col);

/// Переместить курсор на следующую строку с прокруткой
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \return EOK в случае успеха
/// \return ошибку вывода на консоль
inline int mcurses_move_down(mcurses_handle_t handle) {int res = printf("%s", ESC_CUD); if(res > 0) return EOK; return errno;};

/// Переместить курсор на предыдущую строку с прокруткой
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \return EOK в случае успеха
/// \return ошибку вывода на консоль
inline int mcurses_move_up(mcurses_handle_t handle) {int res = printf("%s", ESC_CUU); if(res > 0) return EOK; return errno;};

/// Прокрутить список вниз
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \param [in] int lines - количество строк (по умолчанию 1)
/// \return EOK в случае успеха
/// \return ошибку вывода на консоль
inline int mcurses_scroll_down(mcurses_handle_t handle, int lines = 1) {int res = printf("%s%d%s", ESC_BEGIN, lines, "S"); if(res > 0) return EOK; return errno;};

/// Прокрутить список вверх
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \param [in] int lines - количество строк (по умолчанию 1)
/// \return EOK в случае успеха
/// \return ошибку вывода на консоль
inline int mcurses_scroll_up(mcurses_handle_t handle, int lines = 1) {int res = printf("%s%d%s", ESC_BEGIN, lines, "T"); if(res > 0) return EOK; return errno;};

/// Сохранить позицию и параметры курсора
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \return EOK в случае успеха
/// \return ошибку вывода на консоль
inline int mcurses_save_cursor(mcurses_handle_t handle) {int res = printf("%s", ESC_DECSC); if(res > 0) return EOK; return errno;};

/// Восстановить позицию и параметры курсора
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \return EOK в случае успеха
/// \return ошибку вывода на консоль
inline int mcurses_restore_cursor(mcurses_handle_t handle) {int res = printf("%s", ESC_DECRC); if(res > 0) return EOK; return errno;};

/// Скрыть курсор
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \return EOK в случае успеха
/// \return ошибку вывода на консоль
inline int mcurses_hide_cursor(mcurses_handle_t handle) {int res = printf("%s", ESC_HIDE_CUR); if(res > 0) return EOK; return errno;};

/// Показать курсор
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \return EOK в случае успеха
/// \return ошибку вывода на консоль
inline int mcurses_show_cursor(mcurses_handle_t handle) {int res = printf("%s", ESC_SHOW_CUR); if(res > 0) return EOK; return errno;};

/// Вывести текст на консоль с указанными атрибутами
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \param [in] const char *text - текстовая строка для вывода
/// \param [in] int attr - атрибуты текста (по умолчанию обычный текст)
/// \return EOK в случае успеха
/// \return ошибку вывода на консоль
int mcurses_put_text(mcurses_handle_t handle, const char *text, int attr = ta_normal);

/// Вывести строку на консоль с указанными атрибутами,
/// с предварительной очисткой строки
/// \param [in] mcurses_handle_t handle - дескриптор интерфейса
/// \param [in] const char *text - текстовая строка для вывода
/// \param [in] int attr - атрибуты текста (по умолчанию обычный текст)
/// \return EOK в случае успеха
/// \return ошибку вывода на консоль
int mcurses_put_string(mcurses_handle_t handle, const char *text, int attr = ta_normal);
 
/// Стандартная функция-обработчик нажатий клавиш в меню
/// \param [in] mcurses_handle_t handle - дескриптор меню
/// \param [in] mcurses_key_t *key - описание нажатой клавиши
/// \return 1 для продолжения работы экранного элемента, либо 0 для выхода
inline int mcurses_menu_key_handler(mcurses_handle_t handle, mcurses_key_t *key) {return 1;};

/// Инициализация экранного меню с созданием дескриптора
/// \param [in, out] menu_handle_t *handle - указатель на дескриптор меню
/// \param [in] mcurses_handle_t mcurses - дескриптор интерфейса
/// \param [in] mcurses_menu_item_handler_t item_func - функция-обработчик пункта меню
/// \param [in] const char *header - заголовок меню
/// \param [in] size_t items_count - количество элементов меню (по умолчанию 0)
/// \param [in] mcurses_menu_item_t *items - массив элементов меню (по умолчанию не инициализирован)
/// \param [in] mcurses_key_handler_t key_func - функция-обработчик нажатий клавиш (по умолчанию - стандартный)
/// \return EOK в случае успеха
/// \return EINVAL, если указатели не инициализированы
/// \return признак ошибки
int mcurses_menu_init(menu_handle_t *handle, mcurses_handle_t mcurses, mcurses_menu_item_handler_t item_func, const char *header, size_t items_count = 0,
                      mcurses_menu_item_t *items = NULL,
                      mcurses_key_handler_t key_func = mcurses_menu_key_handler);

/// Закрытие экранного меню
/// \param [in, out] menu_handle_t *handle - указатель на дескриптор меню
/// \return EOK в случае успеха
/// \return EINVAL, если указатели не инициализированы
/// \return признак ошибки
int mcurses_menu_close(menu_handle_t *handle);

/// Добавление пункта меню
/// \param [in] menu_handle_t handle - дескриптор меню
/// \param [in] const char *text - отображаемый текст
/// \param [in] char sym - символ быстрого доступа (0 - отключает быстрый доступ)
/// \param [in] const char *str - хранимая строка
/// \param [in] uint32_t int_data - хранимые целочисленные данные
/// \param [in] float float_data - хранимые данные с плавающей точкой
/// \return EOK в случае успеха
/// \return EINVAL, если указатели не инициализированы
/// \return признак ошибки
int mcurses_menu_add(menu_handle_t handle, const char *text, char sym, const char *str, uint32_t int_data, float float_data);

/// Доступ к пункту меню по порядковому номеру
/// \param [in] menu_handle_t handle - дескриптор меню
/// \param [in] int item - порядковый номер пункта
/// \return указатель на пункт меню
/// \return NULL в случае ошибки
mcurses_menu_item_t *mcurses_menu_item(menu_handle_t handle, int item);

/// Доступ к пункту меню по отображаемому тексту
/// \param [in] menu_handle_t handle - дескриптор меню
/// \param [in] const char *text - отображаемый текст
/// \return указатель на пункт меню
/// \return NULL в случае ошибки
mcurses_menu_item_t *mcurses_menu_by_text(menu_handle_t handle, const char *text);

/// Доступ к пункту меню по символу быстрого доступа
/// \param [in] menu_handle_t handle - дескриптор меню
/// \param [in] char sym - символ быстрого доступа
/// \return указатель на пункт меню
/// \return NULL в случае ошибки
mcurses_menu_item_t *mcurses_menu_by_sym(menu_handle_t handle, char sym);

/// Отображение и запуск меню
/// \param [in] menu_handle_t handle - дескриптор меню
/// \return EOK в случае успеха
/// \return признак ошибки
int mcurses_menu_run(menu_handle_t handle);

/// Отображение меню
/// \param [in] menu_handle_t handle - дескриптор меню
/// \return EOK в случае успеха
/// \return признак ошибки
int mcurses_menu_display(menu_handle_t handle);

/// Отображение пункта меню
/// \param [in] menu_handle_t handle - дескриптор меню
/// \param [in] int item - порядковый номер пункта
/// \param [in] int selected - выделить (1) или нет (0) пункт
/// \return EOK в случае успеха
/// \return признак ошибки
int mcurses_menu_item_display(menu_handle_t handle, int item, int selected);

/// Определение ширины меню
/// \param [in] menu_handle_t handle - дескриптор меню
/// \return EOK в случае успеха
/// \return признак ошибки
int mcurses_menu_define_width(menu_handle_t handle);


};

#pragma pack(pop)

#endif