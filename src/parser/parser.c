#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "../lexer/lexer.h"

extern int print_steps;
static void one_creation_ast(struct ast *ast, FILE* input,struct token **tok);

//initaliasation of an ast
static struct ast *ast_init()
{
    if (print_steps)
        printf("[ast_init] Initializing a new AST node\n");

    struct ast *ast_tok = calloc(1, sizeof(struct ast));
    ast_tok->nb_children = 0;
    ast_tok->children = calloc(1, sizeof(struct ast *));
    ast_tok->data = calloc(1, sizeof(char *));
    ast_tok->type = AST_CMD;

    return ast_tok;
}

// add children to ast
static void add_children(struct ast *ast)
{
    if (print_steps)
        printf("[add_children] Adding a child to the AST node\n");

    ast->nb_children++;
    ast->children = realloc(ast->children, sizeof(struct ast *) * ast->nb_children);
    ast->children[ast->nb_children - 1] = ast_init();
}

// create node command
static void creation_command(struct ast *ast, struct token **tok, FILE *input)
{
    if (print_steps)
        printf("[creation_command] Creating a command node\n");

    ast->data[0] = (*tok)->value;

    if (print_steps)
        printf("  Command: %s\n", ast->data[0]);

    free(*tok);
    *tok = lexer(input);

    int i = 2;

    while (tok && (*tok)->type == WORDS)
    {
        ast->data = realloc(ast->data, sizeof(char *) * i);
        ast->data[i - 1] = (*tok)->value;

        if (print_steps)
            printf("  Arg[%d]: %s\n", i - 1, ast->data[i - 1]);

        free(*tok);
        *tok = lexer(input);
        i++;
    }

    ast->data = realloc(ast->data, sizeof(char *) * i);
    ast->data[i - 1] = NULL;
}


// static void creation_if(struct ast *ast, struct token **tok, FILE *input)
// {
//     ast->type = AST_IF;
//     add_children(ast);
//     struct token *tok = lexer(input);
//     ast->children[0]->data = *tok;
//     ast->children[0]->type = AST_CDT;
//     *tok = lexer(input);
//     add_children(ast);
//     ast->children[1]->data = *tok;
//     creation_then(ast->children[1]); //then
//     int i = 2;
//     while ((*tok)->type == ELIF)
//     {
//         add_children(ast);
//         ast->children[i]->type = IF;
//         ast->children[i]->data = tok;
//         creation_if(ast->children[i],tok, input);
//         i++;
//     }
//     if ((*tok)->type == ELSE)
//     {
//         add_children(ast);
//         creation_then(ast->children[i]); //else
//         i++;
//     }
//     add_children(ast); //next code
//     creation_then(ast->children[i]);
// }

static void one_creation_ast(struct ast *ast, FILE* input,struct token **tok)
{
    // tok WORDS
        if((*tok)->type == WORDS)
        {
            add_children(ast);
            creation_command(ast->children[ast->nb_children - 1], tok,input);
            ast = ast->children[ast->nb_children - 1];
        }
        // next tok can be IF, ... don't use lexer again or free(tok)

        //tok IF
        if ((*tok)->type == IF)
        {
            add_children(ast);
            //creation_if(ast->children[ast->nb_children - 1], &tok,input);
            ast = ast->children[ast->nb_children - 1];
        }
        free(*tok);
        *tok = lexer(input);
}

//create ast 
static void creation_ast(struct ast *ast, FILE* input)
{
    struct token *tok;
    tok = lexer(input);
    while (tok && tok->type != END)
    {
        // tok WORDS
        one_creation_ast(ast,input,&tok);
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
    free(ast->data[j]);
    free(ast->data);
    free(ast);
}

void print_ast(struct ast *ast, int depth)
{
    if (!ast)
        return;

    for (int i = 0; i < depth; i++)
        printf("  ");
    
    printf("Node type: %d\n", ast->type);
    
    if (ast->data)
    {
        for (int i = 0; ast->data[i]; i++)
        {
            for (int j = 0; j < depth + 1; j++)
                printf("  ");
            printf("Data: %s\n", ast->data[i]);
        }
    }

    for (size_t i = 0; i < ast->nb_children; i++)
        print_ast(ast->children[i], depth + 1);
}

