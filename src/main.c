#include "main.h"
#include "io/io.h"
#include "lexer/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include "parser/parser.h"
#include "lecture/lecture.h"

int main(int argc, char **argv)
{
    FILE *input = io(argc, argv);
    if (!input)
        return 1;

    struct token *tok;
    struct ast *ast = lecture(input);
    free_ast(ast);
    return 0;
}

