#include "echo.h"
#include <stdio.h>
#include <string.h>

//standard version of echo
// 1 = -n et 2 = -e
void echo_standard(char *argv, int flags)
{
		for (size_t j = 0; j < strlen(argv); j++)
		{
			if (flags < 2)
			{
				if (argv[j] == '\\' && j != strlen(argv))
				{
					j++;
					if (argv[j] == 'n')
					{
						putchar('\n');
					}
					else if (argv[j] == 't')
						putchar('\t');
					else if (argv[j] == '\\')
						putchar('\\');
					else
					{
						j--;
						putchar(argv[j]);	
					}
				}
				else
					putchar(argv[j]);
			}
			else
				putchar(argv[j]);
		}
	if ((flags % 2) == 1)
		printf("\n");
}

//echo function
//print things
void echo_true(char **argv)
{
	int flags = 0;
	if(argv[1] == NULL)
		return;
	if (argv[1][0] != '-')
	{
		echo_standard(argv[1], flags);
		return;
	}
	for (size_t i = 0; i < strlen(argv[1]); i++)
	{
		if (argv[1][i] == 'n')
		{
			if ((flags % 2) == 0)
				flags++;
		}
		else if (argv[1][i] == 'E')
		{
			if (flags < 2)
				flags+=2;
		}
		else if (argv[1][i] == 'E')
		{
			if (flags >= 2)
				flags-=2;
		}
	}
	echo_standard(argv[2], flags);
}

void echo(char **argv)
{
	int i = 0;
	if (argv[0] == NULL || argv[1] == NULL)
		return;
	else if (argv[1][0] == '-')
		i = 2;
	else
	{
		if (argv[2] == NULL)
			return;
		i = 3;
	}
	while(argv[i] != NULL)
	{
		strcat(argv[2], argv[i]);
		i++;
	}
	echo_true(argv);
}
