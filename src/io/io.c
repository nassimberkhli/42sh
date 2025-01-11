#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>

extern int print_steps;

FILE *io(int argc, char **argv)
{
    if (argc == 1)
    {
        if (print_steps)
            printf("[io] Reading from stdin\n");
        return stdin;
    }
    else if (argc == 2)
    {
        if (print_steps)
            printf("[io] Opening file: %s\n", argv[1]);

        FILE *file = fopen(argv[1], "r");
        if (!file)
        {
            perror("[io] Error opening file");
            return NULL;
        }
        return file;
    }
    else if (argc == 3 && !strcmp(argv[1], "-c"))
    {
        if (print_steps)
            printf("[io] Reading command: %s\n", argv[2]);

        FILE *file = fmemopen(argv[2], strlen(argv[2]), "r");
        if (!file)
        {
            perror("[io] Error opening command file");
            return NULL;
        }
        return file;
    }
    return NULL;
}

