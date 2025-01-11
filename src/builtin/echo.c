#include "echo.h"
#include <stdio.h>
#include <string.h>

//standard version of echo
void echo_standard(char **argv)
{
	int i = 1;
	while(argv[i] != NULL)
	{
		if(i == 1)
			printf("%s", argv[i]);
		else
			printf(" %s", argv[i]);
	}
	printf("\n");
}

//echo but without final endline
void echo_no_space(char **argv)
{	
	int i = 2;
	while(argv[i] != NULL)
	{
		if(i == 2)
			printf("%s", argv[i]);
		else
			printf(" %s", argv[i]);
	}
}

//echo function
//print things
void echo(char **argv)
{
	if (strcmp("-n", argv[0]) == 0)
		echo_no_space(argv);
	else
		echo_standard(argv);
}
