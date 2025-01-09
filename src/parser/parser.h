#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

enum ast_type
{
    AST_CMD,
    AST_IF,
    AST_LIST,
    AST_WHILE,
    AST_UNTIL,
    AST_REDIR,
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


#endif /* ! PARSER_H */