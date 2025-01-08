#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>

FILE* io(int argc, char **argv)
{
    if (argc == 1)
        return stdin;
    else if (argc == 2)
    {
        FILE *file = fopen(argv[1], "r");
        if (!file)
        {
            perror("Error opening file");
            return NULL;
        }
        return file;
    }
    else if (argc == 3 && !strcmp(argv[1],"-c"))
    {
        FILE *file = fmemopen(argv[2], strlen(argv[2]), "r");
        if (!file)
        {
            perror("Error opening command file");
            return NULL;
        }
        return file;
    }
    return NULL;
}

