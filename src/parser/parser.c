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
    ast_tok->type = AST_CMD;
    return ast_tok;
}

// add children to ast
static void add_children(struct ast *ast)
{
    ast->nb_children++;
    ast->children[ast->nb_children - 1] = ast_init();
}

// create node command
static void creation_command(struct ast *ast, struct token **tok, FILE *input)
{
    ast->data= calloc(1, sizeof(char *));
    ast->data[0] = (*tok)->value;
    free(*tok);
    *tok = lexer(input);

    int i = 2;
    // create the data of the node with all words in the command
    while (tok && (*tok)->type == WORDS)
    {
        ast->data = realloc(ast->data, sizeof(char*) * i);
        ast->data[i - 1] = (*tok)->value;
        free(*tok);
        *tok = lexer(input);
        i++;
    }

    ast->data = realloc(ast->data, sizeof(char*) * i);
    ast->data[i - 1] = NULL;

}

//create ast 
static void creation_ast(struct ast *ast, FILE* input)
{
    struct token *tok;
    tok = lexer(input);
    while (tok && tok->type != END)
    {
        // tok WORDS
        if(tok->type == WORDS)
        {
            add_children(ast);
            creation_command(ast->children[ast->nb_children - 1], &tok,input);
            ast = ast->children[ast->nb_children - 1];
        }
        // if '/n' or ';' skip to another node
        if (tok->type == RET_LINE || tok->type == COMMA)
        {
            free(tok);
        }
        tok = lexer(input);
    }
    if (tok)
        free(tok);
}


//main function who call real main function
struct ast *parser(FILE* input)
{
    struct ast *ast_tok = ast_init();
    creation_ast(ast_tok, input);
    return ast_tok;
}

//free the ast 
void free_ast(struct ast *ast)
{
    for (size_t i = 0; i != ast->nb_children ; i++)
    {
        free_ast(ast->children[i]);
    }
    free(ast->children);
    int j = 0;
    for (j = 0; ast->data[j]; j++)
    {
        free(ast->data[j]);
    }
    free(ast->data);
    free(ast);
}