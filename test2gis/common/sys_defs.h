#ifndef __SYS_DEFINITIONS__
#define __SYS_DEFINITIONS__

#if defined(__QNXNTO__)
#if !defined(__EXT_POSIX1_199309)
#define __EXT_POSIX1_199309
#endif
#if !defined(__EXT_POSIX1_198808)
#define __EXT_POSIX1_198808
#endif
#if !defined(_QNX_SOURCE)
#define _QNX_SOURCE
#endif
#endif

#include <stddef.h>
#include <stdint.h>
#include <errno.h>

/** 
\file

\brief ���������� 䠩� � ���ᠭ��� �᭮���� �������

����� 䠩� ᮤ�ন� � ᥡ� ��।������ �᭮����
�������, �㭪権 � ����⠭�, �ᯮ��㥬�� � ��⥬�

*/

#pragma pack(push,1)

/// \defgroup log_returns ����⠭�� ��� ��।������ �����頥��� �����᪨� ���祭��
/// @{
#ifndef FALSE
#define FALSE 0		///< ����
#endif
#ifndef TRUE
#define TRUE  1		///< ��⨭�
#endif
#ifndef NEGATIVE
#define NEGATIVE -1		///< �ਧ��� �訡��
#endif
/// @}

#ifndef EZERO
#define EZERO 0		///< ��।������ �ਧ���� ������⢨� �訡�� (errno == 0)
#endif
#ifndef EOK
#define EOK EZERO	///< ��८�।������ �ਧ���� ������⢨� �訡�� EOK, �᫨ �� �� ��।���� � �⠭���⭮� ���������
#endif

#ifndef __Byte_t__
#define __Byte_t__
/// \defgroup byte_t ��।������ ���⮢��� ⨯�
/// @{

/// ���⮢� ⨯ (�� �� ��� �몠� ���� � �� ����)
typedef uint8_t byte_t;

#define MAX_BYTE_T	255		///< ���ᨬ��쭮� ���祭�� 楫��� �᫠ ���⮢��� ⨯�

/// @}
#endif

#ifndef ___bit_ops
#define ___bit_ops
/// \defgroup bit_ops ����� ��⮢�� ����権
/// @{

/// �஢�ઠ ���ﭨ� ��� � �����
/// \param [in,out] [in] - ����� ��⮢
/// \param [in] bit - ����� ��� � �����, ���� � 0
/// \return 0, �᫨ ��� �� �������, �� 0, �᫨ ��� �������
#define check_bit(in, bit)   (in & (1 << (bit)))

/// ��⠭���� ��� � ����� � �������
/// \param [in,out] [in] - ����� ��⮢
/// \param [in] bit - ����� ��� � �����, ���� � 0
/// \return ���
#define set_bit(in, bit)   (in | (1 << (bit)))

/// ���� ��� � ����� � ���
/// \param [in,out] [in] - ����� ��⮢
/// \param [in] bit - ����� ��� � �����, ���� � 0
/// \return ���
#define clr_bit(in, bit)   (in & ~(1 << (bit)))

/// ������஢���� ��� � �����
/// \param [in,out] [in] - ����� ��⮢
/// \param [in] bit - ����� ��� � �����, ���� � 0
/// \return ���
#define inv_bit(in, bit)   (in ^ (1 << (bit)))

/// �஢�ઠ ����� ��⮢ �� ��᪥
/// \param [in,out] [in] - ����� ��⮢
/// \param [in] mask - ��⮢�� ��᪠
/// \return 0, �᫨ ����� ��⮢ �� ᮮ⢥����� ��᪥, �� 0, �᫨ ᮮ⢥�����
#define check_mask(in, mask)   (in & (mask))

/// ��⠭���� ��⮢ � ����� �� ��᪥
/// \param [in,out] [in] - ����� ��⮢
/// \param [in] mask - ��⮢�� ��᪠
/// \return ���
#define set_mask(in, mask)   (in | (mask))

/// ���� ��⮢ � ����� �� ��᪥
/// \param [in,out] [in] - ����� ��⮢
/// \param [in] mask - ��⮢�� ��᪠
/// \return ���
#define clr_mask(in, mask)   (in & ~(mask))

/// @}
#endif



#ifndef MAX_PATH
#define MAX_PATH         260       ///< ���ᨬ���� ࠧ��� ����� 䠩��
#endif

#ifndef MAX_STRING
#define MAX_STRING       80        ///< ���ᨬ���� ࠧ��� ����ࠪ⭮� ��ப�
#endif

#ifndef MAX_SHORT_STRING
#define MAX_SHORT_STRING 32        ///< ���ᨬ���� ࠧ��� ����ࠪ⭮� ��ப�
#endif

#ifndef MAX_BUFFER_STRING
#define MAX_BUFFER_STRING 1024     ///< ���ᨬ���� ࠧ��� ��ப�, �뤥�塞�� ��� ����
#endif

#ifndef MAX_BUFFER_SIZE
#define MAX_BUFFER_SIZE 8192       ///< ���ᨬ���� ࠧ��� ����ࠪ⭮�� ����
#endif

/// \defgroup millisec ������⢥��� ����⠭��
/// @{
#define MILLISEC     1000           ///< ������⢮ �����ᥪ㭤 � ᥪ㭤�
#define MICROSEC     1000000        ///< ������⢮ ����ᥪ㭤 � ᥪ㭤�
#define NANOSEC      1000000000     ///< ������⢮ ����ᥪ㭤 � ᥪ㭤�
#define NANOMILLISEC 1000000        ///< ������⢮ ����ᥪ㭤 � �����ᥪ㭤�
#define NANOMICROSEC 1000        ///< ������⢮ ����ᥪ㭤 � ����ᥪ㭤�

#define M2U(ms)		(ms * 1000)     ///< ��ॢ�� �����ᥪ㭤 � ����ᥪ㭤�

#define N2U(ns)		(ns / 1000)     ///< ��ॢ�� ����ᥪ㭤 � ����ᥪ㭤�

/// @}

/// \defgroup err_codes �������⥫�� ���� �訡��
/// @{
#define __ETIMEOUT -4            ///< ���祭�� ���ࢠ�� ��������
/// @}

/// \defgroup null_macro ������ ��� ࠡ��� � 㪠��⥫ﬨ
/// @{
#ifndef delete_null
/// �᢮������ ������ �����஬ delete � �஢�મ� 㪠��⥫� �� NULL
/// \param [in,out] item - 㪠��⥫� �� ��ꥪ�
/// \return ���
#define delete_null(item)    {if((item) != NULL) delete (item); (item) = NULL;};
#endif

#ifndef free_null
/// �᢮������ ������ �㭪樥� free � �஢�મ� 㪠��⥫� �� NULL
/// \param [in,out] item - 㪠��⥫� �� ������� �����
/// \return ���
#define free_null(item)    {if((item) != NULL) free((item)); (item) = NULL;};
#endif

#ifndef do_null
/// �믮����� ����� ����⢨�, �᫨ 㪠��⥫� �� ࠢ�� NULL
/// \param [in] item - 㪠��⥫� �� ��ꥪ� ��� ������� �����
/// \param [in] action - ���� ����, ����뢠�騩 ����⢨�, ���ਬ�� i++
/// \return ���
#define do_null(item, action)    {if((item) != NULL) {action;};}
#endif

#ifndef call_null
/// �맢��� ����� �㭪�� ��ꥪ�, �᫨ 㪠��⥫� �� ࠢ�� NULL
/// \param [in] item - 㪠��⥫� �� ��ꥪ�
/// \param [in] func - ���ᠭ�� �맮�� �㭪樨, ���ਬ�� f(x)
/// \return ���
#define call_null(item, func)    {if((item) != NULL) {(item)->func;};}
#endif

#ifndef get_null
/// ������� �᫮��� ᢮��⢮ ��ꥪ�, �᫨ 㪠��⥫� �� ࠢ�� NULL
/// \param [in] item - 㪠��⥫� �� ��ꥪ�
/// \param [in] val - ᢮��⢮ ��ꥪ� ��� ���ᠭ�� �맮�� �㭪樨, ���ਬ�� f(x)
/// \return ���祭�� ᢮��⢠ ��� �����頥��� ���祭�� �㭪樨, �᫨ 㪠��⥫� �� ࠢ�� NULL
/// \return 0, �᫨ 㪠��⥫� ࠢ�� NULL
#define get_null(item, val)    ((item) != NULL ? (item)->val : 0)
#endif

#ifndef get_err_null
/// ������� �᫮��� ᢮��⢮ ��ꥪ� ��� �ਧ��� �訡��, �᫨ 㪠��⥫� �� ࠢ�� NULL
/// \param [in] item - 㪠��⥫� �� ��ꥪ�
/// \param [in] val - ᢮��⢮ ��ꥪ� ��� ���ᠭ�� �맮�� �㭪樨, ���ਬ�� f(x)
/// \return ���祭�� ᢮��⢠ ��� �����頥��� ���祭�� �㭪樨, �᫨ 㪠��⥫� �� ࠢ�� NULL
/// \return ENOMEM (12), �᫨ 㪠��⥫� ࠢ�� NULL
#define get_err_null(item, val)    ((item) != NULL ? (item)->val : ENOMEM)
#endif

#ifndef get_str_null
/// ������� ��ப���� ᢮��⢮ ��ꥪ�, �᫨ 㪠��⥫� �� ࠢ�� NULL
/// \param [in] item - 㪠��⥫� �� ��ꥪ�
/// \param [in] str - ᢮��⢮ ��ꥪ� ��� ���ᠭ�� �맮�� �㭪樨, ���ਬ�� f(x)
/// \return ���祭�� ᢮��⢠ ��� �����頥��� ���祭�� �㭪樨, �᫨ 㪠��⥫� �� ࠢ�� NULL
/// \return ����� ��ப� (""), �᫨ 㪠��⥫� ࠢ�� NULL
#define get_str_null(item, str)    ((item) != NULL ? (item)->str : "")
#endif

#ifndef check_null
/// �஢���� ����� �᫮���, �᫨ 㪠��⥫� �� ࠢ�� NULL
/// \param [in] item - 㪠��⥫� �� ��ꥪ�
/// \param [in] cond - ���� ����, ����뢠�騩 �᫮���, ���ਬ�� handle > -1
/// \return १���� �஢�ન �᫮���, �᫨ 㪠��⥫� �� ࠢ�� NULL
/// \return FALSE (0), �᫨ 㪠��⥫� ࠢ�� NULL
#define check_null(item, cond)    (item != NULL ? (cond) : FALSE)
#endif

/// @}


/// \defgroup misc_macro �������⥫�� ������
/// @{

/// ��������� foreach ��� ������ ���ᨢ��
/// \param [in] type - ⨯ ���ᨢ�
/// \param [in, out] item - ����� ���ᨢ�, �ᯮ��㥬� � 横��
/// \param [in] arr - ���ᨢ
/// \param [in] arrsiz - ࠧ��� ���ᨢ�
/// \param [in] iter - ����� ���ᨢ�, ������� �� �맮�� �����
#define for_each(type, item, arr, arrsiz, iter) (iter) = 0; for( type item=((iter) < (arrsiz) ? arr[(iter)] : 0) ; (iter) < (arrsiz) ; ++(iter), item=((iter) < (arrsiz) ? arr[(iter)] : 0))

/// �८�ࠧ������ ����� � ⥪�⮢�� ��ப�
/// \param [in] s �����
#define STR_MACRO(s) VAL_MACRO(s)

/// ����⠭���� ���祭�� �।��।�������� �����
/// \param [in] s �����
#define VAL_MACRO(s) #s



/// @}


#pragma pack(pop)

#endif