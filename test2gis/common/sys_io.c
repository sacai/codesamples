/** @file
 Функции чтения/записи в файл, с контролем прерывания по сигналу
 */

#include "sys_io.h"
#include "util.h"

/////////////////////////////////////////////////////////////////////////////////////
/// Открытие файла с контролем прерывания по сигналу
/// \param int *handle - указатель на переменную под дескриптор файла
/// \param char *name - имя файла
/// \param int mode - режим открытия файла
/// \param int access - маска прав на доустп к файлу
/// \return 0 (EOK) в случае успеха
/// \return EINVAL, если указатели не инициализированы
/// \return ошибку открытия
int open_r(int *handle, char *name, int mode, int access)
{
int result = EINVAL;
int o_hndl = -1;

if(handle != NULL && name != NULL)
{
	while(o_hndl < 0)
	{
		o_hndl = open(name, mode, access);
		if(o_hndl < 0)
		{
			if(errno != EINTR)
			{
				result = errno;
				break;
			}
		}
		else
		    result = EOK;
	}
	fcntl(o_hndl, F_SETFD, FD_CLOEXEC);
	*handle = o_hndl;
}

return(result);
}
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
/// Чтение файла с контролем прерывания по сигналу
/// \param int handle - дескриптор файла
/// \param void *buffer - указатель на буфер для записи в файл
/// \param size_t size - размер буфера для записи в файл
/// \param off_t *r_bytes - указатель на переменную под количество прочитанных байт
/// \return 0 (EOK) в случае успеха
/// \return ENOENT, если файл не открыт
/// \return EINVAL, если указатель на буфер не инициализирован
/// \return ошибку чтения, если не удалось прочитать необходимое количество данных,
/// и при этом чтение не прерывалось сигналом
int read_r(int handle, void *buffer, size_t size, off_t *r_bytes)
{
int result = EINVAL;
byte_t *buf_ptr = (byte_t *)buffer;
off_t rd_bytes = 0, to_read = size;
off_t r_bytes_buffer = 0;

if(handle > -1 && buffer != NULL && size > 0)
{
	result = EOK;
	while(to_read > 0)
	{
		errno = EOK;
		rd_bytes = read(handle, buf_ptr, to_read);
		if(rd_bytes == to_read)
		{
			result = EOK;
			r_bytes_buffer += rd_bytes;
			break;
		}
		else
		{
			if(rd_bytes >= 0)
			{
				if(errno == EINTR || errno == EAGAIN)
				{
					to_read -= rd_bytes;
					buf_ptr += rd_bytes;
					r_bytes_buffer += rd_bytes;
				}
				else
				{
					result = EOK;
					r_bytes_buffer += rd_bytes;
					break;
				}
			}
			else
			{
				if(errno != EINTR)
				{
					result = errno;
					break;
				}
			}
		}
	}
	if(r_bytes)
		*r_bytes = r_bytes_buffer;
}

return(result);
}
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
/// Запись файла с контролем прерывания по сигналу
/// \param int handle - дескриптор файла
/// \param void *buffer - указатель на буфер для записи в файл
/// \param size_t size - размер буфера для записи в файл
/// \param off_t *w_bytes - указатель на переменную под количество прочитанных байт
/// \return 0 (EOK) в случае успеха
/// \return ENOENT, если файл не открыт
/// \return EINVAL, если указатель на буфер не инициализирован
/// \return ошибку записи, если не удалось записать необходимое количество данных,
/// и при этом запись не прерывалось сигналом
int write_r(int handle, void *buffer, size_t size, off_t *w_bytes)
{
int result = EINVAL;
byte_t *buf_ptr = (byte_t *)buffer;
off_t wr_bytes = 0, to_write = size;
off_t w_bytes_buffer = 0;

if(handle > -1 && buffer != NULL && size > 0)
{
	result = EOK;
	while(to_write > 0)
	{
		errno = EOK;
		wr_bytes = write(handle, buf_ptr, to_write);
		if(wr_bytes == to_write)
		{
			result = EOK;
			w_bytes_buffer += wr_bytes;
			break;
		}
		else
		{
			if(wr_bytes >= 0)
			{
				if(errno == EINTR || errno == EAGAIN)
				{
					to_write -= wr_bytes;
					buf_ptr += wr_bytes;
					w_bytes_buffer += wr_bytes;
				}
				else
				{
					result = EOK;
					w_bytes_buffer += wr_bytes;
					break;
				}
			}
			else
			{
				if(errno != EINTR)
				{
					result = errno;
					break;
				}
			}
		}
	}
	if(w_bytes)
		*w_bytes = w_bytes_buffer;
}

return(result);
}
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
/// Закрытие файла с контролем прерывания по сигналу
/// \param int handle - дескриптор файла
/// \return 0 (EOK) в случае успеха
/// \return ошибку закрытия
int close_r(int handle)
{
int result = EINVAL;
int closed = -1;

if(handle > -1)
{
	while(closed < 0)
	{
		closed = close(handle);
		if(closed < 0)
		{
			if(errno != EINTR)
			{
				result = errno;
				break;
			}
		}
		else
		    result = EOK;
	}
}

return(result);
}
/////////////////////////////////////////////////////////////////////////////////////
