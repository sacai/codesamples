#include <string.h>
//#include <malloc.h>

/** \file
	\brief Реализация интерфейса для разбора структурированных текстовых файлов настройки.
	
	Реализация интерфейса для разбора структурированных текстовых файлов настройки на строки, а строк - на поля.
	Файл должен состоять из текстовых строк, поля которых разделены заданными символами (например, табуляцией).
	Поля могут быть заключены в пары символов-ограничителей, например, квадратные скобки [], в этом случае все символы,
	заключенные внутри этих ограничителей, а также сама пара ограничителей, рассматриваются как одно поле, например [это поле].
	
	Кроме того, интерфейс предоставляет средства для разбора простейших математических выражений с константами, например 2+2.
	
	Интерфейс наследуется от базового класса TextFile.

*/

#include "sys_mem.h"
#include "textbase.h"

#define COMMENT                 "#"        ///< символ начала комментария
#define DIVIDER                 "\t ="     ///< символы - разделители
#define PARENTSESS             "[{(<]})>"  ///< парные разделители
#define QUOTER                 "\"\'"      ///< символы -  квотеры
#define CONTINUER               "+\\"      ///< символ продолжения предыдущей строки

/// Конструктор по умолчанию
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

/// Конструктор
/// \param [in] char *fname имя файла
/// \param [in] int mode режим открытия файла, по умолчанию только для чтения
/// \param [in] int access режим доступа к файлу, если он создается, по умолчанию полный доступ
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

/// Деструктор
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

/// Разобрать строку на поля
/// \param [in] char *str анализируемая строка или NULL для обращения к следующему полю в строке
/// \return указатель на очередное вхождения поля в строке, либо NULL, если строка закончилась
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

/// Считать любой символ из строки-параметра символом комментария.
/// по умолчанию используется символ '#'
/// \param [in] const char *s строка символов комментария
/// \return нет
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

/// То же саме, что setQuoter, но задаются
/// парные разделители полей. В строке параметров
/// сначала должны быть первые символы пар
/// затем вторые, по умолчанию  [{(<]})>
/// \param [in] const char *s строка парных скобок
/// \return нет
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

/// Считать любой символ из строки-параметра символом комментария.
/// по умолчанию используется символ '#'
/// \param [in] const char *s строка символов комментария
/// \return нет
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

/// Игнорировать все символы-разделители
/// внутри пары любого символа из строки-
/// параметра. Такая пара символов считается
/// ограничителем поля.
/// по умолчанию используются одинарные и двойные кавычки
/// \param [in] const char *s строка символов-кавычек
/// \return нет
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

/// Считать любой символ из строки-параметра символом продолжения строки.
/// по умолчанию используются символы '+', '\'
/// \param [in] const char *s строка символов продолжения строки
/// \return нет
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


/// Точка входа анализатора математических выражений
/// \param [in, out] char *line - строка выражения и буфер под строку результата
/// \return указатель на результат вычислений в виде текста  
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

/// Получение лексемы из строки
/// \param [in] char *expr - строка выражения
/// \return нет  
void TextBase::getToken(char *expr)
{
math_type=Empty;
memset(math_token, 0, sizeof(math_token));

if(expr[math_token_no]=='\0') //Если конец выражения
{
    math_token_no=0;
    return;
}
while(isspace(expr[math_token_no])) math_token_no++; //Пропустить разделительные символы

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

/// Обрабатывает сложение и вычитание
/// \param [in] double *anw - значение
/// \return 0, если выражение обработано  
/// \return 1, в случае ошибки  
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

/// Обрабатывает умножение и деление
/// \param [in] double *anw - значение
/// \return 0, если выражение обработано  
/// \return 1, в случае ошибки  
int TextBase::fMulti(double *anw)
{
char op;
double temp;
if(fExp(anw)) return 1; //Ошибка

while((op = *math_token) == '*' || op == '/' || op == '%')
{
    getToken(math_expr);
    if(fExp(&temp)) return 1; //Ошибка
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

/// Возведение в степень
/// \param [in] double *anw - значение
/// \return 0, если выражение обработано  
/// \return 1, в случае ошибки  
int TextBase::fExp(double *anw)
{
double temp;
if(fUnary(anw)) return 1; //Ошибка

while(*math_token  == '^')
{
    getToken(math_expr);
    if(fUnary(&temp)) return 1; //Ошибка
    *anw = pow(*anw, temp);
}

return 0;
}

/// Обработка унарных операторов
/// \param [in] double *anw - значение
/// \return 0, если выражение обработано  
/// \return 1, в случае ошибки  
int TextBase::fUnary(double *anw)
{
char op=0;
if(*math_token == '+' || *math_token == '-')
{
    op = *math_token;
    getToken(math_expr);
}
if(fBrack(anw)) return 1; //Ошибка

if(op == '-') *anw = -(*anw);

return 0;
}

/// Обрабатывает выражение в скобках
/// \param [in] double *anw - значение
/// \return 0, если выражение обработано  
/// \return 1, в случае ошибки  
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
    if(fAtom(anw)) return 1; //Ошибка

return 0;
}

/// Получает значение числа
/// \param [in] double *anw - значение
/// \return 0, если выражение обработано  
/// \return 1, в случае ошибки  
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
