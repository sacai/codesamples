#include "sys_mem.h"
#include "cmdline.h"
#include "util.h"

/** @file
  �࣠������ ࠧ��� ��������� ��ப� �ਫ������
*/

///////////////////////////////////////////////////////////////////////////////
/// ���樠������ ��� ��ࠡ�⪨ �⠭���⭮� ��������� ��ப�
/// \param int cnt ������⢮ ��㬥�⮢ ��������� ��ப�
/// \param char **arg ���ᨢ ��㬥�⮢
/// \param unsigned char s ᯮᮡ �।�⠢����� ��㬥�⮢ � ��������� ��ப�
/// \return 1 � ��砥 �ᯥ�, 0 � ��砥 �訡��
int CommandLine::Init(int cnt, char **arg, unsigned char s)
{
int res;

fromString = FALSE;
setOpt("+");
// ����ࠨ���� ������⢮ ��㬥�⮢,
setCount(cnt);
// �� ���譥� �।�⠢�����,
setStyle(s);
// � 㪠��⥫� �� ᯨ᮪
args = NULL;
res = DRTSMallocZero(args, sizeof(char *)*cnt);
for(int i = 0; i < getCount() && res == EOK; i++)
{
	res = DRTSMallocZero(args[i], 1024);
	if(res == EOK)
    	ExpandEnv(arg[i], args[i], 1024);
}
if(res == EOK)
{
	// �饬 � ᯨ᪥ ��㬥�⮢ ��� 䠩�� ��ࠬ��஢
	char fileName[MAX_PATH];
	memset(fileName, 0, sizeof(fileName));
	getStringParameter("+", 0, fileName, sizeof(fileName));
	if(strlen(fileName))
	{
    	// �����६ 䠩� ��ࠬ��஢
    	CommandFile *file = new CommandFile(fileName);
    	if(file)
    	{
        	int newcount = getCount();
        	if(file->scan(8192, this))
        	{
            	setCount(newcount);
        	}
        	delete_null(file);
    	}
	}
	return(TRUE);
}
return(FALSE);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// ���樠������ ��� ��ࠡ�⪨ ��������� ��ப� Windows (��㬥��� ����� ᯫ�譮� ��ப��)
/// \param char *string ��ப� ��㬥�⮢
/// \param unsigned char s ᯮᮡ �।�⠢����� ��㬥�⮢ � ��������� ��ப�
/// \return 1 � ��砥 �ᯥ�, 0 � ��砥 �訡��
int CommandLine::Init(char *string, unsigned char s)
{
int quote = 0;
int cnt = 0;
char *start = string;
char *end = string;
int res = ENOMEM;

args = NULL;

fromString = TRUE;

// ��������� ��������� ��ப�
for(int i = 0; i < strlen(string); i++)
{
    end = &string[i];
    if(string[i] == '\"')
    {
        if(quote)
            quote = 0;
        else
            quote = 1;
    }
    if(!quote && isspace(string[i]))
    {
        cnt++;
        if(args == NULL)
			res = DRTSMallocZero(args, sizeof(char *)*cnt);
        else
			res = DRTSReallocZero(args, sizeof(char *)*(cnt-1), sizeof(char *)*cnt);
        if(res == EOK)
        {
			res = DRTSMallocZero(args[cnt - 1], end - start + 1);
            if(res == EOK)
            {
                strncpy(args[cnt - 1], start, end - start);
            }
			else
				return(FALSE);
        }
		else
			return(FALSE);
        while(isspace(string[i])) i++;
        start = end = &string[i];
    }
}
end = &string[strlen(string)];
if(start != end)
{
    cnt++;
    if(args == NULL)
		res = DRTSMallocZero(args, sizeof(char *)*cnt);
    else
		res = DRTSReallocZero(args, sizeof(char *)*(cnt-1), sizeof(char *)*cnt);
    if(res == EOK)
    {
		res = DRTSMallocZero(args[cnt - 1], end - start + 1);
        if(res == EOK)
        {
            strncpy(args[cnt - 1], start, end - start);
        }
		else
			return(FALSE);
    }
	else
		return(FALSE);
}
setCount(cnt);

// ����ࠨ���� ���譥� �।�⠢����� ��㬥�⮢,
setStyle(s);

setOpt("+");

// �饬 � ᯨ᪥ ��㬥�⮢ ��� 䠩�� ��ࠬ��஢
char fileName[MAX_PATH];
memset(fileName, 0, sizeof(fileName));
getStringParameter("+", 0, fileName, sizeof(fileName));
if(strlen(fileName))
{
    // �����६ 䠩� ��ࠬ��஢
    CommandFile *file = new CommandFile(fileName);
    if(file)
    {
        int newcount = getCount();
        if(file->scan(8192, this))
        {
            setCount(newcount);
        }
        delete_null(file);
    }
}
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// �����⨥
void CommandLine::Close(void)
{
if(args)
{
    for(int i = 0; i < getCount(); i++)
    {
		DRTSFree(args[i]);
    }
	DRTSFree(args);
}
fromString = FALSE;
setCount(0);
setStyle(0);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// ���������� ��㬥�� � ���ᨢ ��㬥�⮢
/// \param char *argstr ��ப� ��㬥��
/// \return 1 � ��砥 �ᯥ�, 0 � ��砥 �訡��
int CommandLine::addArg(char *argstr)
{
int res = ENOMEM;
int count = getCount();
count++;
if(args == NULL)
	res = DRTSMallocZero(args, sizeof(char *)*count);
else
	res = DRTSReallocZero(args, sizeof(char *)*(count-1), sizeof(char *)*count);
if(res == EOK)
{
    res = DRTSStrdup(args[count - 1], argstr);
    if(res == EOK)
    {
        setCount(count);
        return 1;
    }
}
return 0;
}
///////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
/// ��������� ����� CommandFile
/// \param char *name ��� 䠩��
/// \param int mode ०�� ������ 䠩��
/// \param int access ०�� ����㯠 � 䠩��
CommandFile::CommandFile(char *name, int mode, int access)
         : TextBase(name, mode, access)
{
setDivider("= \t");
setQuoter("\'\"");
cmd = NULL;
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// �������� ����� CommandFile
CommandFile::~CommandFile()
{
cmd = NULL;
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// �����஢���� 䠩�� ��ࠬ��஢
/// \param int bufsize ࠧ��� ���� �� ᪠��஢���� � ࠧ��� 䠩��
/// \param CommandLine *cml ��ꥪ� ��������� ��ப�, � ������ ��������� ��ࠬ����
/// \return 1 � ��砥 �ᯥ�, 0 � ��砥 �訡��
int CommandFile::scan(int bufsize, CommandLine *cml)
{
if(cml != NULL)
{
    // ���������� ⥪�饥 ������⢮ ��ࠬ��஢ � 㪠��⥫� �� ���ᨢ
    cmd = cml;
    // ������㥬 䠩�
    if(TextFile::scan(bufsize) == TRUE)
    {
        return 1;
    }
}
setError(errno);
return 0;
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// ������ ��ப� 䠩�� ��ࠬ��஢
/// \param char *str ��ப� 䠩�� ��ࠬ��஢
/// \param int32_t pos ������ ��ப� � 䠩��
/// \param int len ����� ��ப� � 䠩��
/// \return 0 � ��砥 �ᯥ�, -1 � ��砥 �訡��, �ॡ��饩 �४�饭�� ࠧ��� 䠩��
int CommandFile::parse(char *string, int32_t pos, int len)
{
int res;
char *str = NULL;

if(string && *string != 0 && !isComment(*string))
{
	res = DRTSMallocZero(str, 1024);
	if(res != EOK)
    {
        setError(errno);
        return -1;
    }
    char *field = strtok(string);
    while(field)
    {
        field = Trim(field);
        ExpandEnv(field, str, 1024);
        if(cmd)
        {
            if(!cmd->addArg(str))
            {
                setError(errno);
                DRTSFree(str);
                return -1;
            }
        }
        field = strtok(NULL);
    }
    DRTSFree(str);
}
return 0;
}
///////////////////////////////////////////////////////////////////////////////

