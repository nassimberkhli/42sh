#include "main.h"
#include "io/io.h"
#include "lexer/lexer.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    
    struct token *tok;
    while(tok = lexer(io(argc, argv)))
    {
        printf("%s\n",tok->value);
    }
    return 0;
}

