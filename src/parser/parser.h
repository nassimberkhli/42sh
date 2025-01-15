#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

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
    AST_REDIR,
    AST_PIPELINE,
    AST_APP
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

#endif /* ! PARSER_H */
