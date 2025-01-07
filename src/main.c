#include "main.h"

#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <script_file>\n", argv[0]);
        return 1;
    }

    printf("Hello world!\n");
    return 0;
}

