#include "sys_mem.h"
#include "appcmdl.h"
#include "util.h"

///////////////////////////////////////////////////////////////////////////////
/// �஢�ઠ ��।���� ��㬥�� �� ����稥 ���祢��� ᫮��                  //
/// \param 
/// char *key - ���祢�� ᫮��
/// \param 
/// char **arg - ���ᨢ ��㬥�⮢ ��������� ��ப�
/// \return
/// 1, �᫨ ���祢�� ᫮�� �������, � ��⨢��� ��砥 0
///////////////////////////////////////////////////////////////////////////////
int AppCommandLine::checkKey(char *key, char **arg)
{
// ����ந� 㪠��⥫� �� ��㬥��
char *argstr = *arg;
int index = 0, i = 0;
// ������ ��६����� ��� १���� �ࠢ�����
int res = -1;
// �஢�ਬ ����稥 ���祢��� ᫮��
if(key != NULL)
{
    // ���祢�� ᫮�� ������
	// ��।���� ������ ���� � ᯨ᪥ �����⨬�� ���祩
	for(index = 0; index < opts.size(); index++)
	{
		res = -1;
	    if(style & CASE_SENS)
    	    // �ࠢ������, ���뢠� ॣ���� �㪢
        	res = strcmp(key, opts[index]);
    	else
        	// �ࠢ������, �� ���뢠� ॣ���� �㪢
        	res = Stricmp(key, opts[index]);
		if(!res)
			break;
	}
    // �஢�ਬ ����稥 ��䨪�, ᮮ⢥�����饣�
    // ��������� ���譥�� �।�⠢�����
    if((style & DASH_PREFIX && *argstr == '-')
      || (style & SLASH_PREFIX && *argstr == '/'))
    {
        // �ய��⨬ ��䨪�
        argstr++;
        while((style & DASH_PREFIX) && *argstr == '-') argstr++;
        while((style & SLASH_PREFIX) && *argstr == '/') argstr++;
    }
	res = -1;
    if(style & CASE_SENS)
        // �ࠢ������, ���뢠� ॣ���� �㪢
        res = strncmp(argstr, key, strlen(key));
    else
        // �ࠢ������, �� ���뢠� ॣ���� �㪢
        res = Strnicmp(argstr, key, strlen(key));
    if(!res)
    {
		// ���祢�� ᫮��, ��������, �������, �஢�ਬ, �� ᮢ������ �� ⥪�騩 ���� � ��㣨�
		for(i = 0; i < opts.size(); i++)
		{
			res = -1;
		    if(style & CASE_SENS)
        	    // �ࠢ������, ���뢠� ॣ���� �㪢
            	res = strncmp(argstr, opts[i], strlen(opts[i]));
        	else
            	// �ࠢ������, �� ���뢠� ॣ���� �㪢
        		res = Strnicmp(argstr, opts[i], strlen(opts[i]));
			if(!res)
			{
				// �᫨ ᮢ������� �������, � ���� �� ��, �� �� �᪠�� ᥩ��, ��室�� � ����� १���⮬
				if(i != index)
				{
				    if(style & CASE_SENS)
        		    	// �ࠢ������, ���뢠� ॣ���� �㪢
            			res = strncmp(key, opts[i], strlen(opts[i]));
        			else
            			// �ࠢ������, �� ���뢠� ॣ���� �㪢
           				res = Strnicmp(key, opts[i], strlen(opts[i]));
					if(res < 0)
						return 0;
				}
			}
		}
        // ���祢�� ᫮�� �������, ����ந� 㪠��⥫� ��
        // ��ࠬ���
        argstr+=strlen(key);
        if(*argstr && strchr(CMD_DIV, *argstr))
            argstr++;
        *arg = argstr;
        return 1;
    }
}
else if(key == NULL)
{
    // �஢�ਬ ����稥 ��䨪�, ᮮ⢥�����饣�
    // ��������� ���譥�� �।�⠢�����
    if((style & DASH_PREFIX && *argstr == '-')
      || (style & SLASH_PREFIX && *argstr == '/'))
        return 0;
    // �᫨ ��䨪� �� �����㦥� � �� �⮬ �� ������ ���祢�� ᫮��,
    // १���� �㤥� ������⥫��, ��᪮��� �� ����砥�, ��
    // �� ������ ��ࠬ��� ��� ����
    return 1;
}
// ���祢�� ᫮�� �� �������
return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// ������� ���祭�� ��ࠬ��� �� ���祢��� ᫮�� � ��������� �ଠ��        //
/// \param 
/// const char *key - ���祢�� ᫮�� ��� ᨬ���
/// int next - ���� ������� �� ��砫� ��������� ��ப� (0) ���� �� ��᫥����� ���������� ��㬥�� (1)
/// const char *format - ��ப� �ଠ� ��� ����뢠��� ��㬥��
/// void *param - 㪠��⥫� �� ���祭�� ��㬥��
/// \return
/// ������ ���������� ��㬥�� � ���������� ��ப�, ���� -1 � ��砥 �訡��
///////////////////////////////////////////////////////////////////////////////
int AppCommandLine::getParameter(const char *key, int next, const char *format,
                              void *param)
{
char *arg;            // �����⥫� �� ⥪�騩 ��ᬠ�ਢ���� ��㬥��
int found = 0;         // ������ �� �᪮�� ��ࠬ���
char *keyptr = (char *)key;    // �����⥫� �� ���祢�� ᫮��
// � ����ᨬ��� �� ���祭�� next ���� � 横�� ���� ��
// ��᫥����� ���������� ��㬥�� (����樮��� ᨭ⠪��),
// ���� �� ��砫� ᯨ᪠ ��㬥�⮢
for(int i = (next? current+1 : 0);i < count;i++)
{
    // ����ந� 㪠��⥫� �� ⥪�騩 ��㬥��
    arg = getArg(i);
    if(arg != NULL)
    {
        // � �஢��塞 �� ����稥 ���祢��� ᫮��; �᫨ ����,
        // � 㪠��⥫� ᤢ������� �� ���祭�� ��ࠬ���
        found = checkKey(keyptr, &arg);
        if(found)
        {
            if(!*arg)
            {
                // ������ ⮫쪮 ����, �㤥� �᪠�� ᫥���騩
                // ��ࠬ���, ����� �� ���� ���箬
                keyptr = NULL;
                continue;
            }
            else if(*arg)
            {
				// �஢�ਬ ���祭�� ��㬥��, ��� ������ ���� �᫮��
                if(!isdigit(*arg) && *arg != '-' && *arg != '+')
					continue;
                // ������� ���祭�� �����, ⮫쪮 ���� �ଠ�,
                // ����� �������� �� ⥬ �� �ࠢ����, �� � ���
                // �㭪権 ᥬ���⢠ scanf()
                if(*format == '%')
                {
					int scanned = sscanf(arg, format, param);
					if(scanned != 1)
						return -1;
                    // �������� � ��୥� ������ ��ࠡ�⠭���� ��㬥��
                    current = i;
                    return(current);
                }
            }
        }
    }
}
// �᫨ �� ��諨 �������� ���祢�� ᫮�� ���� �� ᬮ��� �८�ࠧ�����
// ���祭��, ��୥� �ਧ��� �訡��
return -1;
}

///////////////////////////////////////////////////////////////////////////////
/// ������� ���祭�� ��ப����� ��ࠬ���                                    //
/// \param 
/// const char *key - ���祢�� ᫮�� ��� ᨬ���
/// int next - ���� ������� �� ��砫� ��������� ��ப� (0) ���� �� ��᫥����� ���������� ��㬥�� (1)
/// char *param - 㪠��⥫� �� ��ப�-�ਥ���� ���祭�� ��㬥��
/// unsigned size - ࠧ��� ��ப�-�ਥ�����
/// \return
/// ������ ���������� ��㬥�� � ���������� ��ப�, ���� -1 � ��砥 �訡��
///////////////////////////////////////////////////////////////////////////////
int AppCommandLine::getStringParameter(const char *key, int next,
                                    char *param, unsigned size)
{
char *arg;                  // �����⥫� �� ⥪�騩 ��ᬠ�ਢ���� ��㬥��
int found = 0;              // ������ �� �᪮�� ��ࠬ���                   
char *keyptr = (char *)key;    // �����⥫� �� ���祢�� ᫮��
// � ����ᨬ��� �� ���祭�� next ���� � 横�� ���� ��
// ��᫥����� ���������� ��㬥�� (����樮��� ᨭ⠪��),
// ���� �� ��砫� ᯨ᪠ ��㬥�⮢
for(int i = (next? current+1 : 0);i < count;i++)
{
    // ����ந� 㪠��⥫� �� ⥪�騩 ��㬥��
    arg = getArg(i);
    if(arg != NULL)
    {
        // � �஢��塞 �� ����稥 ���祢��� ᫮��; �᫨ ����,
        // � 㪠��⥫� ᤢ������� �� ���祭�� ��ࠬ���
        found = checkKey(keyptr, &arg);
        if(found)
        {
            if(!*arg)
            {
                // ������ ⮫쪮 ����, �㤥� �᪠�� ᫥���騩
                // ��ࠬ���, ����� �� ���� ���箬
                keyptr = NULL;
                continue;
            }
            else if(*arg)
            {
                // �����㥬 � �।��⠢����� ����
                strncpyEx(param, arg, size);
                // �������� � ��୥� ������ ��ࠡ�⠭���� ��㬥��
                current = i;
                return(current);
            }
        }
    }
}
// �᫨ �� ��諨 �������� ���祢�� ᫮�� ���� �� ᬮ��� �८�ࠧ�����
// ���祭��, ��୥� �ਧ��� �訡��
return -1;
}

///////////////////////////////////////////////////////////////////////////////
/// �஢���� ����稥 ��ࠬ���-䫠��                                         //
/// \param 
/// const char *key - ���祢�� ᫮�� ��� ᨬ���
/// int next - ���� ������� �� ��砫� ��������� ��ப� (0) ���� �� ��᫥����� ���������� ��㬥�� (1)
/// int *param - 㪠��⥫� �� ��६�����, �ਬ����� ���祭�� 1, �᫨ ��ࠬ��� ������, � 0 � ��⨢��� ��砥
/// \return
/// ������ ���������� ��㬥�� � ���������� ��ப�, ���� -1 � ��砥 �訡��
///////////////////////////////////////////////////////////////////////////////
int AppCommandLine::getFlagParameter(const char *key, int next, int *param)
{
char *arg;                  // �����⥫� �� ⥪�騩 ��ᬠ�ਢ���� ��㬥��
int found = 0;              // ������ �� �᪮�� ��ࠬ���                   
// � ����ᨬ��� �� ���祭�� next ���� � 横�� ���� ��
// ��᫥����� ���������� ��㬥�� (����樮��� ᨭ⠪��),
// ���� �� ��砫� ᯨ᪠ ��㬥�⮢
for(int i = (next? current+1 : 0);i < count;i++)
{
    // ����ந� 㪠��⥫� �� ⥪�騩 ��㬥��
    arg = getArg(i);
    if(arg != NULL)
    {
        // � �஢��塞 �� ����稥 ���祢��� ᫮��
        found = checkKey((char *)key, &arg);
        if(found)
        {
            // ���� ��������� � ��������� ��ப�,
            // ����ᨬ 1 � ��।���� ����
            *param = 1;
            // �������� � ��୥� ������ ��ࠡ�⠭���� ��㬥��
            current = i;
            return(current);
        }
    }
}
// �᫨ �� ��諨 �������� ���祢�� ᫮�� ���� �� ᬮ��� �८�ࠧ�����
// ���祭��, ��୥� �ਧ��� �訡��
return -1;
}


///////////////////////////////////////////////////////////////////////////////
/// �����஢��� �������� ������⢮ ᨬ����� ��ப� �� ��� ��� ��ॢ��� ��ப�
/// \param 
/// char *dst - ����-�ਥ����
/// \param 
/// const char *src - ��ப�-���筨�
/// \param 
/// size_t n - ������⢮ �����㥬�� ᨬ�����
/// \return
/// 㪠��⥫� �� ����-�ਥ���� ��� NULL � ��砥 ���ᯥ�
char *AppCommandLine::strncpyEx(char *dst, const char *src, size_t n)
{
int i = 0;
if(src && dst)
{
    for(i = 0; i < n && i < strlen(src); i++)
    {
        if(src[i] == '\r' || src[i] == '\n')
            break;
        dst[i] = src[i];
    }
    dst[i] = 0;
    return dst;
}
return NULL;
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// ��⠭����� ����� �����⨬�� ��権
/// \param 
/// const char *opt - ��ப� ���祩 � ���祢�� ᫮�, ࠧ�������� ������ﬨ
/// \return
/// ���
void AppCommandLine::setOpt(const char *opt)
{
if(opt)
{
	int added = 0;
	char *tmpopt = NULL;
	int res = DRTSStrdup(tmpopt, opt);
	if(res == EOK)
	{
		char *curopt = strtok(tmpopt, ":");
		int found, i;
		while(curopt)
		{
			for(i = 0; i < opts.size(); i++)
			{
				if(!strcmp(opts[i], curopt))
					break;
			}
			if(i == opts.size())
			{
				opts.add(curopt);
				added = 1;
			}
			curopt = strtok(NULL, ":");
		}
		if(added)
			opts.sort();
		DRTSFree(tmpopt);
	}
}
}
///////////////////////////////////////////////////////////////////////////////

