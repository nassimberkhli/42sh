#include <stdio.h>

FILE* io(int argc, char **argv)
{
    if (argc == 1)
        return stdin;

    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        perror("Error opening file");
        return NULL;
    }
    return file;
}

