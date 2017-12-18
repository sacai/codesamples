/** @file
 �㭪樨 �⥭��/����� � 䠩�, � ����஫�� ���뢠��� �� ᨣ����
 */

#include "sys_io.h"
#include "util.h"

/////////////////////////////////////////////////////////////////////////////////////
/// ����⨥ 䠩�� � ����஫�� ���뢠��� �� ᨣ����
/// \param int *handle - 㪠��⥫� �� ��६����� ��� ���ਯ�� 䠩��
/// \param char *name - ��� 䠩��
/// \param int mode - ०�� ������ 䠩��
/// \param int access - ��᪠ �ࠢ �� ����� � 䠩��
/// \return 0 (EOK) � ��砥 �ᯥ�
/// \return EINVAL, �᫨ 㪠��⥫� �� ���樠����஢���
/// \return �訡�� ������
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
/// �⥭�� 䠩�� � ����஫�� ���뢠��� �� ᨣ����
/// \param int handle - ���ਯ�� 䠩��
/// \param void *buffer - 㪠��⥫� �� ���� ��� ����� � 䠩�
/// \param size_t size - ࠧ��� ���� ��� ����� � 䠩�
/// \param off_t *r_bytes - 㪠��⥫� �� ��६����� ��� ������⢮ ���⠭��� ����
/// \return 0 (EOK) � ��砥 �ᯥ�
/// \return ENOENT, �᫨ 䠩� �� �����
/// \return EINVAL, �᫨ 㪠��⥫� �� ���� �� ���樠����஢��
/// \return �訡�� �⥭��, �᫨ �� 㤠���� ������ ����室���� ������⢮ ������,
/// � �� �⮬ �⥭�� �� ���뢠���� ᨣ�����
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
/// ������ 䠩�� � ����஫�� ���뢠��� �� ᨣ����
/// \param int handle - ���ਯ�� 䠩��
/// \param void *buffer - 㪠��⥫� �� ���� ��� ����� � 䠩�
/// \param size_t size - ࠧ��� ���� ��� ����� � 䠩�
/// \param off_t *w_bytes - 㪠��⥫� �� ��६����� ��� ������⢮ ���⠭��� ����
/// \return 0 (EOK) � ��砥 �ᯥ�
/// \return ENOENT, �᫨ 䠩� �� �����
/// \return EINVAL, �᫨ 㪠��⥫� �� ���� �� ���樠����஢��
/// \return �訡�� �����, �᫨ �� 㤠���� ������� ����室���� ������⢮ ������,
/// � �� �⮬ ������ �� ���뢠���� ᨣ�����
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
/// �����⨥ 䠩�� � ����஫�� ���뢠��� �� ᨣ����
/// \param int handle - ���ਯ�� 䠩��
/// \return 0 (EOK) � ��砥 �ᯥ�
/// \return �訡�� �������
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
