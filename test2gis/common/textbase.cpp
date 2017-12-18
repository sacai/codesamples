#include <string.h>
//#include <malloc.h>

/** \file
	\brief ��������� ����䥩� ��� ࠧ��� �������஢����� ⥪�⮢�� 䠩��� ����ன��.
	
	��������� ����䥩� ��� ࠧ��� �������஢����� ⥪�⮢�� 䠩��� ����ன�� �� ��ப�, � ��ப - �� ����.
	���� ������ ������ �� ⥪�⮢�� ��ப, ���� ������ ࠧ������ ������묨 ᨬ������ (���ਬ��, ⠡��樥�).
	���� ����� ���� �����祭� � ���� ᨬ�����-��࠭��⥫��, ���ਬ��, ������� ᪮��� [], � �⮬ ��砥 �� ᨬ����,
	�����祭�� ����� ��� ��࠭��⥫��, � ⠪�� ᠬ� ��� ��࠭��⥫��, ��ᬠ�ਢ����� ��� ���� ����, ���ਬ�� [�� ����].
	
	�஬� ⮣�, ����䥩� �।��⠢��� �।�⢠ ��� ࠧ��� ���⥩�� ��⥬���᪨� ��ࠦ���� � ����⠭⠬�, ���ਬ�� 2+2.
	
	����䥩� ��᫥����� �� �������� ����� TextFile.

*/

#include "sys_mem.h"
#include "textbase.h"

#define COMMENT                 "#"        ///< ᨬ��� ��砫� ���������
#define DIVIDER                 "\t ="     ///< ᨬ���� - ࠧ����⥫�
#define PARENTSESS             "[{(<]})>"  ///< ���� ࠧ����⥫�
#define QUOTER                 "\"\'"      ///< ᨬ���� -  ������
#define CONTINUER               "+\\"      ///< ᨬ��� �த������� �।��饩 ��ப�

/// ��������� �� 㬮�砭��
TextBase::TextBase()
{
Divider=Comment=Quoter=Parentsess=Continuer=NULL;
divLen=commLen=quotLen=parLen=contLen=0;

setDivider(DIVIDER);
setParentsess(PARENTSESS);
setComment(COMMENT);
setQuoter(QUOTER);
setContinuer(CONTINUER);
}

/// ���������
/// \param [in] char *fname ��� 䠩��
/// \param [in] int mode ०�� ������ 䠩��, �� 㬮�砭�� ⮫쪮 ��� �⥭��
/// \param [in] int access ०�� ����㯠 � 䠩��, �᫨ �� ᮧ������, �� 㬮�砭�� ����� �����
TextBase::TextBase(char *fname, int mode, int access)
         : TextFile(fname,mode,access)
{
Divider=Comment=Quoter=Parentsess=Continuer=NULL;
divLen=commLen=quotLen=parLen=contLen=0;

setDivider(DIVIDER);
setParentsess(PARENTSESS);
setComment(COMMENT);
setQuoter(QUOTER);
setContinuer(CONTINUER);
}

/// ��������
TextBase::~TextBase()
{
if(Divider)
{
    free(Divider);
    divLen=0;
    Divider = NULL;
}
if(Comment)
{
    free(Comment);
    commLen=0;
    Comment = NULL;
}
if(Quoter)
{
    free(Quoter);
    quotLen=0;
    Quoter = NULL;
}
if(Parentsess)
{
    free(Parentsess);
    parLen=0;
    Parentsess = NULL;
}
if(Continuer)
{
    free(Continuer);
    contLen=0;
    Continuer = NULL;
}
}

/// �������� ��ப� �� ����
/// \param [in] char *str ��������㥬�� ��ப� ��� NULL ��� ���饭�� � ᫥���饬� ���� � ��ப�
/// \return 㪠��⥫� �� ��।��� �宦����� ���� � ��ப�, ���� NULL, �᫨ ��ப� �����稫���
char* TextBase::strtok(char *str)
{
static char *current = NULL;

char *start=NULL;
char *Quote=NULL;
char *Par=NULL;
char *cp = NULL;

if(str) current=str;

while(current && isDiv(*current)) current++;
if(current && *current) start=current;
else return NULL;

while(current && *current)
{
	if((Par=isPar(*current))!=NULL)
	{
		if((cp=(char*) memchr(current+1,*(Par+4),strlen(current+1)))!=NULL)
		{
			if((*(cp+1))!='\0') current=cp+2;
			else current=cp+1;
			*(cp+1)='\0';
			return start;
		}
	}
	if((Quote=isQuote(*current))!=NULL)
	{

		if((cp=(char*) memchr(current+1,*Quote,strlen(current+1)))!=NULL)
		{
			start=current+1;
			*cp='\0';
			current=cp+1;
			return start;
	  	}
		else current++;
		continue;
	}

	if(isDiv(*current))
	{
		*current='\0';
		current++;
		return start;
	}
	current++;
}
return start;
}

/// ����� �� ᨬ��� �� ��ப�-��ࠬ��� ᨬ����� ���������.
/// �� 㬮�砭�� �ᯮ������ ᨬ��� '#'
/// \param [in] const char *s ��ப� ᨬ����� ���������
/// \return ���
void TextBase::setDivider(const char *s)
{
if(Divider)
{
    DRTSFree(Divider);
    divLen=0;
}
int res = DRTSStrdup(Divider, s);
if(res == EOK)
    divLen=strlen(s);
else
    setError(res);
}

/// �� �� ᠬ�, �� setQuoter, �� ��������
/// ���� ࠧ����⥫� �����. � ��ப� ��ࠬ��஢
/// ᭠砫� ������ ���� ���� ᨬ���� ���
/// ��⥬ ����, �� 㬮�砭��  [{(<]})>
/// \param [in] const char *s ��ப� ����� ᪮���
/// \return ���
void TextBase::setParentsess(const char *s)
{
if(Parentsess)
{
    DRTSFree(Parentsess);
    parLen=0;
}
int res = DRTSStrdup(Parentsess, s);
if(res == EOK)
{
    parLen=strlen(s);
    parLen/=2;
}
else
    setError(res);
}

/// ����� �� ᨬ��� �� ��ப�-��ࠬ��� ᨬ����� ���������.
/// �� 㬮�砭�� �ᯮ������ ᨬ��� '#'
/// \param [in] const char *s ��ப� ᨬ����� ���������
/// \return ���
void TextBase::setComment(const char *s)
{
if(Comment)
{
    DRTSFree(Comment);
    commLen=0;
}
int res = DRTSStrdup(Comment, s);
if(res == EOK)
    commLen=strlen(s);
else
    setError(res);
}

/// �����஢��� �� ᨬ����-ࠧ����⥫�
/// ����� ���� ��� ᨬ���� �� ��ப�-
/// ��ࠬ���. ����� ��� ᨬ����� ��⠥���
/// ��࠭��⥫�� ����.
/// �� 㬮�砭�� �ᯮ������� ������� � ������ ����窨
/// \param [in] const char *s ��ப� ᨬ�����-����祪
/// \return ���
void TextBase::setQuoter(const char *s)
{
if(Quoter)
{
    DRTSFree(Quoter);
    quotLen=0;
}
int res = DRTSStrdup(Quoter, s);
if(res == EOK)
    quotLen=strlen(s);
else
    setError(res);
}

/// ����� �� ᨬ��� �� ��ப�-��ࠬ��� ᨬ����� �த������� ��ப�.
/// �� 㬮�砭�� �ᯮ������� ᨬ���� '+', '\'
/// \param [in] const char *s ��ப� ᨬ����� �த������� ��ப�
/// \return ���
void TextBase::setContinuer(const char *s)
{
if(Continuer)
{
    DRTSFree(Continuer);
    contLen=0;
}
int res = DRTSStrdup(Continuer, s);
if(res == EOK)
    contLen=strlen(s);
else
    setError(res);
}


/// ��窠 �室� ��������� ��⥬���᪨� ��ࠦ����
/// \param [in, out] char *line - ��ப� ��ࠦ���� � ���� ��� ��ப� १����
/// \return 㪠��⥫� �� १���� ���᫥��� � ���� ⥪��  
char *TextBase::math_pars(char *line)
{
double result;
math_err=No;
math_expr=line;
math_token_no = 0;
math_type = Empty;
getToken(math_expr);
fSum(&result);

switch(math_err)
{
	case No:
		sprintf(math_expr, "%f", result);
	break;
	case Syntax:
		strcpy(math_expr, "Syntax error!");
	break;
	case Zero:
		strcpy(math_expr, "Divide by zero!");
	break;
}
setError(math_err);
math_token_no = 0;
memset(math_token, 0, sizeof(math_token));
return math_expr;
}

/// ����祭�� ���ᥬ� �� ��ப�
/// \param [in] char *expr - ��ப� ��ࠦ����
/// \return ���  
void TextBase::getToken(char *expr)
{
math_type=Empty;
memset(math_token, 0, sizeof(math_token));

if(expr[math_token_no]=='\0') //�᫨ ����� ��ࠦ����
{
    math_token_no=0;
    return;
}
while(isspace(expr[math_token_no])) math_token_no++; //�ய����� ࠧ����⥫�� ᨬ����

if(strchr("+-*/%^=()", expr[math_token_no]))
{
    *math_token = expr[math_token_no];
    *(math_token+1) = '\0';
    math_type = Operator;
}
else if(isalpha(expr[math_token_no]) || expr[math_token_no] == '_' || expr[math_token_no] == '$')
{
    int j=0;
    math_token[j]=expr[math_token_no];
    while(isalpha(expr[math_token_no+1]) || expr[math_token_no+1] == '_' || expr[math_token_no+1] == '$')
        math_token[++j]=expr[++math_token_no];
    math_token[j+1]='\0';
    math_type = Variable;
}
else if(isdigit(expr[math_token_no]))
{
    int j=0;
    math_token[j]=expr[math_token_no];
    while(isdigit(expr[math_token_no+1])||expr[math_token_no+1]=='.')
        math_token[++j]=expr[++math_token_no];
    math_token[j+1]='\0';
    math_type=Number;
}
math_token_no++;
}

/// ��ࠡ��뢠�� ᫮����� � ���⠭��
/// \param [in] double *anw - ���祭��
/// \return 0, �᫨ ��ࠦ���� ��ࠡ�⠭�  
/// \return 1, � ��砥 �訡��  
int TextBase::fSum(double *anw)
{
char op;
double temp;
if(fMulti(anw)) return 1;

while((op = *math_token) == '+' || op == '-')
{
    getToken(math_expr);
    fMulti(&temp);
    switch(op)
    {
     case '+':
        *anw += temp;
        break;
     case '-':
        *anw -= temp;
        break;
    }
}

return 0;
}

/// ��ࠡ��뢠�� 㬭������ � �������
/// \param [in] double *anw - ���祭��
/// \return 0, �᫨ ��ࠦ���� ��ࠡ�⠭�  
/// \return 1, � ��砥 �訡��  
int TextBase::fMulti(double *anw)
{
char op;
double temp;
if(fExp(anw)) return 1; //�訡��

while((op = *math_token) == '*' || op == '/' || op == '%')
{
    getToken(math_expr);
    if(fExp(&temp)) return 1; //�訡��
    switch(op)
    {
     case '*':
        *anw *= temp;
        break;
     case '/':
        if(temp == 0.0)
        {
            math_err=Zero;
            return 1;
        }
        *anw /= temp;
        break;
     case '%':
        *anw = (int)*anw % (int)temp;
        break;
    }
}

return 0;
}

/// ���������� � �⥯���
/// \param [in] double *anw - ���祭��
/// \return 0, �᫨ ��ࠦ���� ��ࠡ�⠭�  
/// \return 1, � ��砥 �訡��  
int TextBase::fExp(double *anw)
{
double temp;
if(fUnary(anw)) return 1; //�訡��

while(*math_token  == '^')
{
    getToken(math_expr);
    if(fUnary(&temp)) return 1; //�訡��
    *anw = pow(*anw, temp);
}

return 0;
}

/// ��ࠡ�⪠ 㭠��� �����஢
/// \param [in] double *anw - ���祭��
/// \return 0, �᫨ ��ࠦ���� ��ࠡ�⠭�  
/// \return 1, � ��砥 �訡��  
int TextBase::fUnary(double *anw)
{
char op=0;
if(*math_token == '+' || *math_token == '-')
{
    op = *math_token;
    getToken(math_expr);
}
if(fBrack(anw)) return 1; //�訡��

if(op == '-') *anw = -(*anw);

return 0;
}

/// ��ࠡ��뢠�� ��ࠦ���� � ᪮����
/// \param [in] double *anw - ���祭��
/// \return 0, �᫨ ��ࠦ���� ��ࠡ�⠭�  
/// \return 1, � ��砥 �訡��  
int TextBase::fBrack(double *anw)
{
if(*math_token == '(')
{
    getToken(math_expr);
    fSum(anw);

    if(*math_token != ')')
    {
        math_err=Syntax;
        return 1;
    }
    getToken(math_expr);
}
else
    if(fAtom(anw)) return 1; //�訡��

return 0;
}

/// ����砥� ���祭�� �᫠
/// \param [in] double *anw - ���祭��
/// \return 0, �᫨ ��ࠦ���� ��ࠡ�⠭�  
/// \return 1, � ��砥 �訡��  
int TextBase::fAtom(double *anw)
{
if(math_type == Number)
{
    *anw = atof(math_token);
    getToken(math_expr);
}
else if(math_type == Variable)
{
    *anw = getMathVar(math_token);
	if(getError() == EINVAL)
	{
        math_err=Syntax;
	    return 1;
	}
    getToken(math_expr);
}
else
{
    math_err=Syntax;
    return 1;
}

return 0;
}
