/**
  @file Тестовое задание

Напишите (консольную) программу, принимающую на вход  имя файла и набор параметров.
В зависимости от параметров программа должна работать в трёх режимах:
1)      test -f Test.tst -m words -v mother
печатает количество слов "mother" в файле "Test.tst"
2)      test -f Test.tst -m checksum
печатает 32-битную чексумму, рассчитанную по алгоритму checksum = word1 + word2 + ... + wordN
(word1..wordN - 32-хбитные слова, представляющие содержимое файла)
3)      test -h
печатает информацию о программе и описание параметров.

*/

#include <stdio.h>
#include "cmdline.h"
#include "findword.h"
#include "checksum.h"

/// Режим работы
enum WorkMode {modeNone = 0, modeWordCount, modeChkSum, modeMax};

/// Наименования режимов работы
const char *WorkModeName[modeMax] = {"", "words", "checksum"};

//////////////////////////////////////////////////////////////////////////////
/// Стандартная точка входа приложения
int main(int argc, char *argv[])
{
	int result = EOK;
	WorkMode mode = modeNone;
	char mode_str[MAX_PATH] = {0};
	char word[MAX_PATH] = {0};
	char file[MAX_PATH] = {0};
	int word_count = 0;
	uint32_t chksum = BAD_CHECKSUM;
	int help = 0;
	const char *UseMsg = "Usage:\n"
						"test2gis -f <file> -m words -v <word_to_find>\n"
						"<file> - file full name\n"
						"<word_to_find> - word to find in file\n"
						"prints found words count\n"
						"test2gis -f <file> -m checksum\n"
						"<file> - file full name\n"
						"prints checksum\n";
	// Разбор командной строки
	CommandLine AppCmd;
	AppCmd.Init(argc, argv, UNIX_LIKE);
	AppCmd.setOpt("f:m:v:h");
	AppCmd.getFlagParameter("h", 0, &help);
	if(!help)
	{
		AppCmd.getStringParameter("f", 0, file, sizeof(file)-1);
		if(strlen(file))
		{
			AppCmd.getStringParameter("m", 0, mode_str, sizeof(mode_str)-1);
			if(strlen(mode_str))
			{
				if(!strcmp(mode_str, WorkModeName[modeWordCount]))
				{
					mode = modeWordCount;
					AppCmd.getStringParameter("v", 0, word, sizeof(word)-1);
					if(!strlen(word))
					{
						result = EINVAL;
						printf("No word to seek!\n");
					}
				}
				else if(!strcmp(mode_str, WorkModeName[modeChkSum]))
					mode = modeChkSum;
			}
			else
			{
				result = EINVAL;
				printf("Empty mode identifier!\n");
			}
		}
		else
		{
			result = EINVAL;
			printf("Empty file name!\n");
		}
	}
	else
	{
		printf("%s\n", UseMsg);
		return(EOK);
	}
	if(result == EOK)
	{
		if(mode == modeWordCount)
		{
			FindWord seeker(file);
			if(seeker.getError() == EOK)
			{
				word_count = seeker.findword(32767, word);
				if(word_count > 0)
					printf("Word \"%s\" count in file \"%s\":%d\n", word, file, word_count);
				else if(word_count == 0)
					printf("Word \"%s\" is not found in file \"%s\"\n", word, file);
				else
					printf("Error %d seeking word \"%s\" in file \"%s\"\n", seeker.getError(), file);
			}
		}
		if(mode == modeChkSum)
		{
			CheckSum counter(file);
			if(counter.getError() == EOK)
			{
				chksum = counter.get_checksum();
				if(chksum != BAD_CHECKSUM)
					printf("Checksum of file \"%s\":%u\n", file, chksum);
				else
					printf("Error %d calculating checksum of file \"%s\"\n", counter.getError(), file);
			}
		}
	}

	return(result);
}
//////////////////////////////////////////////////////////////////////////////

