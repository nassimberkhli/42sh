#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "../lexer/lexer.h"

//initaliasation of an ast
static struct ast* ast_init()
{
    struct ast * ast_tok = calloc(1, sizeof(struct ast));
    ast_tok->nb_children = 0;
    ast_tok->children = calloc(1, sizeof(struct ast *));
    ast_tok->data = calloc(1, sizeof(char *));
    return ast_tok;
}


//construct ast for command
static void ast_word(struct token * token, struct ast * ast)
{
    int i = 0;
    ast->data = realloc(ast->data, sizeof(ast->data) + strlen(token->value) + 2);
    while (ast->data[i] != NULL)
        i++;
    ast->data[i] = token->value;
    ast->type = AST_CMD;
}

//create child for comma or return line
static void ast_comma(struct ast * ast)
{
    ast->nb_children++;
    ast->children = realloc(ast->children, sizeof(struct ast) * ast->nb_children);
    ast->children[0] = ast_init();
}


//real main fuctionè&
//create ast depand on the type of token
static void ast_child(struct ast *ast, FILE * input)
{
    struct token * token = lexer(input);
    while(token != NULL)
    {
        if (token->type == WORDS)
            ast_word(token, ast);
        else if (token->type == COMMA || token->type == RET_LINE)
        {
            ast_comma(ast);
            ast_child(ast->children[0], input);
            return;
        }
        token = lexer(input);
    }
}


//main function who call real main function
struct ast *parser(FILE* input)
{
    struct ast * ast_tok = ast_init();
    ast_child(ast_tok, input);
    return ast_tok;
}