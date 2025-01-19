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
    NUMBER_REDIR,
    REDIR_IN,
    REDIR_IN_APP,
    REDIR_OUT,
    REDIR_OUT_APP,
    DUP_IN,
    DUP_OUT,
    OPEN_RW,
    PIPELINE,
    END,
    AND,
    OR
};


struct token 
{
    enum token_type type;
    char *value;
};

struct token *lexer(FILE *input);

#endif /* ! LEXER_H */
