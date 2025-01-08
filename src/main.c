#include "main.h"
#include "io/io.h"
#include "lexer/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include "parser/parser.h"

int main(int argc, char **argv)
{
    FILE *input = io(argc, argv);
    if (!input)
        return 1;

    struct token *tok;
    struct ast *ast = parser(input);
    while ((tok = lexer(input)))
    {
        printf("%s\n", tok->value);
        free(tok);
    }
    ast++;
    return 0;
}

