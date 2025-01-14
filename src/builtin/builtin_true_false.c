#include "builtin_true_false.h"

#include <stddef.h>

// Builtin_TRUE
int builtin_true(char **argv)
{
    if (argv != NULL)
    {
        if (argv[0] != NULL)
        {
            return 0;
        }
    }
    return 0;
}

// Builtin_FALSE
int builtin_false(char **argv)
{
    if (argv != NULL)
    {
        if (argv[0] != NULL)
        {
            return 1;
        }
    }
    return 1;
}
