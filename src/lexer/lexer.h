#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

enum token_type
{
    IF,
    FI,
    THEN,
    ELIF,
    ELSE,
    COMMA,
    RET_LINE,
    FOR,
    WHILE,
    UNTIL,
    DO,
    DONE,
    QUOTE,
    WORDS,
    REDIR,
    APP,
    PIPELINE,
    END
};


struct token 
{
    enum token_type type;
    char *value;
};

struct token *lexer(FILE *input);

#endif /* ! LEXER_H */
