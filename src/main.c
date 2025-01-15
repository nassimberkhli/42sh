#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io/io.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "lecture/lecture.h"
#include "variable/hash_map.h"
#include "variable/variable.h"

int print_steps = 0;
struct hash_map *hash_map = NULL;

int main(int argc, char **argv)
{
    hash_map = hash_map_init(32);
    // special_variable();
    if (argc > 1 && strcmp(argv[1], "-p") == 0)
    {
        print_steps = 1;
        argc--;
        argv++;
    }

    FILE *input = io(argc, argv);
    if (!input)
        return 1;

    struct ast *ast = lecture(input);
    free_ast(ast);
    return atoi(get_variable("$?"));
}

