#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <stdio.h>

enum ast_type
{
    AST_CMD,
    AST_CONDITION,
    AST_THEN,
    AST_ELIF,
    AST_ELSE,
    AST_IF,
    AST_CDT,
    AST_WHILE,
    AST_UNTIL,
    AST_REDIR_OUT,
    AST_REDIR_OUT_APP,
    AST_REDIR_IN,
    AST_REDIR_IN_APP,
    AST_DUP_IN,
    AST_DUP_OUT,
    AST_OPEN_RW,
    AST_PIPELINE,
    AST_APP,
    AST_AND,    
    AST_OR
};

struct ast *parser(FILE* input);
void free_ast(struct ast *ast);

struct ast
{
    enum ast_type type;

    size_t nb_children;
    struct ast **children;
    char **data;
};

void print_ast(struct ast *ast, int depth);
void free_ast(struct ast *ast);

#endif /* ! PARSER_H */
