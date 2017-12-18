#if !defined(__TEXTBASE_H)
#define __TEXTBASE_H

/** \file
	\brief ��������� ����䥩� ��� ࠧ��� �������஢����� ⥪�⮢�� 䠩��� ����ன��.
	
	��������� ����䥩� ��� ࠧ��� �������஢����� ⥪�⮢�� 䠩��� ����ன�� �� ��ப�, � ��ப - �� ����.
	���� ������ ������ �� ⥪�⮢�� ��ப, ���� ������ ࠧ������ ������묨 ᨬ������ (���ਬ��, ⠡��樥�).
	���� ����� ���� �����祭� � ���� ᨬ�����-��࠭��⥫��, ���ਬ��, ������� ᪮��� [], � �⮬ ��砥 �� ᨬ����,
	�����祭�� ����� ��� ��࠭��⥫��, � ⠪�� ᠬ� ��� ��࠭��⥫��, ��ᬠ�ਢ����� ��� ���� ����, ���ਬ�� [�� ����].
	
	�஬� ⮣�, ����䥩� �।��⠢��� �।�⢠ ��� ࠧ��� ���⥩�� ��⥬���᪨� ��ࠦ���� � ����⠭⠬�, ���ਬ�� 2+2.
	
	����䥩� ��᫥����� �� �������� ����� TextFile.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "textfile.h"

#pragma pack(push,1)

extern "C" {

enum {Empty = 0, Operator, Variable, Number}; //��� ���ᥬ�
enum {No = EOK, Zero = EFAULT, Syntax = EINVAL}; //���祭�� �訡��

///////////////////////////////////////////////////////////////////////////
///   \brief ������ ����� TextBase �।��⠢��� �㭪樨 ��� ࠧ��� ��ப
/// ⥪�⮢��� 䠩�� �� ����.
///   \details �������-ࠧ����⥫� ����� ����� ����������.
/// ���� ����� ���� �����祭� � ���� ᨬ�����-��࠭��⥫��, ���ਬ��, ������� ᪮��� [], � �⮬ ��砥 �� ᨬ����,
///	�����祭�� ����� ��� ��࠭��⥫��, � ⠪�� ᠬ� ��� ��࠭��⥫��, ��ᬠ�ਢ����� ��� ���� ����, ���ਬ�� [�� ����].
///	�஬� ⮣�, �।��⠢������ �।�⢠ ��� ࠧ��� ���⥩�� ��⥬���᪨� ��ࠦ���� � ������ १���� � ���� ��ப�,
/// ���ਬ�� TextBase::math_pars("2+2") ��୥� ��ப� "4"
///
/// � �ந������� ����� ������ ���� ��८�।����� �㭪�� TextFile::parse
class TextBase : public TextFile
{
  int divLen;   ///< ����� ��ப� ࠧ����⥫��
  int commLen;  ///< ����� ��ப� ������ਥ�
  int quotLen;  ///< ����� ��ப� ᨬ����� ����祪
  int parLen;   ///< ����� ��ப� ᨬ����� ����� ᪮���
  int contLen;  ///< ����� ��ப� ᨬ����� �த������� ��ப�

protected:

  char *Divider;    ///< �����⥫� �� ��ப� ࠧ����⥫��               
  char *Comment;    ///< �����⥫� ��ப� ������ਥ�                
  char *Quoter;     ///< �����⥫� ��ப� ᨬ����� ����祪           
  char *Parentsess; ///< �����⥫� ��ப� ᨬ����� ����� ᪮���     
  char *Continuer;  ///< �����⥫� ��ப� ᨬ����� �த������� ��ப�

  char *math_expr; ///< �����⥫� �� ৡ�ࠥ��� ��ࠦ����
  int math_token_no;   ///< ����� ��।���� ����� ��⥬���᪮�� ��ࠦ����
  int math_type;   ///< ⨯ ��।���� ����� ��⥬���᪮�� ��ࠦ����
  char math_token[MAX_PATH]; ///<���ᥬ�
  int math_err;   ///< �訡�� ࠧ��� ��⥬���᪮�� ��ࠦ����

public:
  /// �஢�ઠ, ���� �� ����� ᨬ��� ࠧ����⥫��
  /// \param [in] char a �஢��塞� ᨬ���
  /// \return 1, �᫨ ᨬ��� ���� ࠧ����⥫��, � ��⨢��� ��砥 0
  int isDiv(char a) {  if(memchr(Divider,a,divLen)!=NULL) return 1;
					   else return 0;
					};

  /// �஢�ઠ, ���� �� ����� ᨬ��� ��砫�� ���������
  /// \param [in] char b �஢��塞� ᨬ���
  /// \return 1, �᫨ ᨬ��� ���� ��砫�� ���������, � ��⨢��� ��砥 0
  int isComment(char b) {
						 if(memchr(Comment,b,commLen)!=NULL) return 1;
						 else return 0;
						};

  /// �஢�ઠ, ���� �� ����� ᨬ��� �ਧ����� �த������� ��ப�
  /// \param [in] char b �஢��塞� ᨬ���
  /// \return 1, �᫨ ᨬ��� ���� �ਧ����� �த�������, � ��⨢��� ��砥 0
  int isCont(char b) { if(memchr(Continuer,b,contLen)!=NULL) return 1;
               else return 0;
                    };

  /// �஢�ઠ, ���� �� ����� ᨬ��� ����窠��
  /// \param [in] char c �஢��塞� ᨬ���
  /// \return 㪠��⥫� �� ��ࢮ� �宦����� ᨬ����, � ��⨢��� ��砥 NULL
  char *isQuote(char c) { return((char *)memchr(Quoter,c,quotLen));};

  /// �஢�ઠ, ���� �� ����� ᨬ��� ����� �� ����� ᪮���
  /// \param [in] char c �஢��塞� ᨬ���
  /// \return 㪠��⥫� �� ��ࢮ� �宦����� ᨬ����, � ��⨢��� ��砥 NULL
  char *isPar(char c)   { return((char *)memchr(Parentsess,c,parLen));};

  /// ����� �� ᨬ��� �� ��ப�-��ࠬ��� ࠧ����⥫�� �����.
  /// �� 㬮�砭�� �ᯮ������� ᨬ���� '\t', ' ', '='
  /// \param [in] const char *s ��ப� ࠧ����⥫��
  /// \return ���
  void setDivider(const char *s);

  /// ����� �� ᨬ��� �� ��ப�-��ࠬ��� ᨬ����� ���������.
  /// �� 㬮�砭�� �ᯮ������ ᨬ��� '#'
  /// \param [in] const char *s ��ப� ᨬ����� ���������
  /// \return ���
  void setComment(const char *s);

  /// �����஢��� �� ᨬ����-ࠧ����⥫�
  /// ����� ���� ��� ᨬ���� �� ��ப�-
  /// ��ࠬ���. ����� ��� ᨬ����� ��⠥���
  /// ��࠭��⥫�� ����.
  /// �� 㬮�砭�� �ᯮ������� ������� � ������ ����窨
  /// \param [in] const char *s ��ப� ᨬ�����-����祪
  /// \return ���
  void setQuoter(const char *s);

  /// �� �� ᠬ�, �� setQuoter, �� ��������
  /// ���� ࠧ����⥫� �����. � ��ப� ��ࠬ��஢
  /// ᭠砫� ������ ���� ���� ᨬ���� ���
  /// ��⥬ ����, �� 㬮�砭��  [{(<]})>
  /// \param [in] const char *s ��ப� ����� ᪮���
  /// \return ���
  void setParentsess(const char *s);

  /// ����� �� ᨬ��� �� ��ப�-��ࠬ��� ᨬ����� �த������� ��ப�.
  /// �� 㬮�砭�� �ᯮ������� ᨬ���� '+', '\'
  /// \param [in] const char *s ��ப� ᨬ����� �த������� ��ப�
  /// \return ���
  void setContinuer(const char *s);

  /// �������� ��ப� �� ����
  /// \param [in] char *str ��������㥬�� ��ப� ��� NULL ��� ���饭�� � ᫥���饬� ���� � ��ப�
  /// \return 㪠��⥫� �� ��।��� �宦����� ���� � ��ப�, ���� NULL, �᫨ ��ப� �����稫���
  char* strtok(char *str);

/// \defgroup math_parse �㭪樨 ࠧ��� ��⥬���᪨� ��ࠦ����
/// @{

  /// ����祭�� ���ᥬ� �� ��ப�
  /// \param [in] char *expr - ��ப� ��ࠦ����
  /// \return ���  
  void getToken(char *expr);

  /// ��窠 �室� ��������� ��⥬���᪨� ��ࠦ����
  /// \param [in, out] char *line - ��ப� ��ࠦ���� � ���� ��� ��ப� १����
  /// \return 㪠��⥫� �� १���� ���᫥��� � ���� ⥪��  
  char *math_pars(char *line);

  /// ��ࠡ��뢠�� ᫮����� � ���⠭��
  /// \param [in] double *anw - ���祭��
  /// \return 0, �᫨ ��ࠦ���� ��ࠡ�⠭�  
  /// \return 1, � ��砥 �訡��  
  int fSum(double *anw);

  /// ��ࠡ��뢠�� 㬭������ � �������
  /// \param [in] double *anw - ���祭��
  /// \return 0, �᫨ ��ࠦ���� ��ࠡ�⠭�  
  /// \return 1, � ��砥 �訡��  
  int fMulti(double *anw);

  /// ���������� � �⥯���
  /// \param [in] double *anw - ���祭��
  /// \return 0, �᫨ ��ࠦ���� ��ࠡ�⠭�  
  /// \return 1, � ��砥 �訡��  
  int fExp(double *anw);

  /// ��ࠡ�⪠ 㭠��� �����஢
  /// \param [in] double *anw - ���祭��
  /// \return 0, �᫨ ��ࠦ���� ��ࠡ�⠭�  
  /// \return 1, � ��砥 �訡��  
  int fUnary(double *anw);

  /// ��ࠡ��뢠�� ��ࠦ���� � ᪮����
  /// \param [in] double *anw - ���祭��
  /// \return 0, �᫨ ��ࠦ���� ��ࠡ�⠭�  
  /// \return 1, � ��砥 �訡��  
  int fBrack(double *anw);

  /// ����砥� ���祭�� �᫠
  /// \param [in] double *anw - ���祭��
  /// \return 0, �᫨ ��ࠦ���� ��ࠡ�⠭�  
  /// \return 1, � ��砥 �訡��  
  int fAtom(double *anw);

  /// �㭪�� ������ �������� ���祭�� ��६����� �� �� ����� � ��ࠦ����.
  /// ��८�।������ � �ந������� �����
  /// \param [in] char *name - ��� ��६�����
  /// \return ���祭�� ��६�����  
  virtual double getMathVar(char *name) {return 0.0;};

/// @}

  /// ��������� �� 㬮�砭��
  TextBase();

  /// ���������
  /// \param [in] char *fname ��� 䠩��
  /// \param [in] int mode ०�� ������ 䠩��, �� 㬮�砭�� ⮫쪮 ��� �⥭��
  /// \param [in] int access ०�� ����㯠 � 䠩��, �᫨ �� ᮧ������, �� 㬮�砭�� ����� �����
  TextBase(char *fname, int mode = O_RDONLY, int access = 0777);

  /// ��������
  ~TextBase();

};

};

#pragma pack(pop)

#endif