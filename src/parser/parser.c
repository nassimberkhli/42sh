#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "../lexer/lexer.h"

extern int print_steps;
static void one_creation_ast(struct ast *ast, FILE* input, struct token **tok);
static void creation_if(struct ast *ast, FILE* input, struct token **tok);

// Initialisation of an AST
static struct ast *ast_init()
{
    if (print_steps)
    {
        printf("[ast_init] Initializing a new AST node\n");
    }
    struct ast *ast_tok = calloc(1, sizeof(struct ast));
    ast_tok->nb_children = 0;
    ast_tok->children = calloc(1, sizeof(struct ast *));
    ast_tok->data = calloc(1, sizeof(char *));
    ast_tok->type = AST_CMD;
    return ast_tok;
}

// Add Children to AST
static void add_children(struct ast *ast, struct ast *child_ast)
{
    if (print_steps)
    {
        printf("[add_children] Adding a child to the AST node\n");
    }
    ast->nb_children++;
    ast->children = realloc(ast->children, sizeof(struct ast *) * ast->nb_children);
    ast->children[ast->nb_children - 1] = child_ast;
}

// Create Node Command
static void creation_command(struct ast *ast, struct token **tok, FILE *input)
{
    if (print_steps)
    {
        printf("[creation_command] Creating a command node\n");
    }
    ast->data[0] = (*tok)->value;

    if (print_steps)
    {
        printf("  Command: %s\n", ast->data[0]);
    }
    free(*tok);
    *tok = lexer(input);

    int i = 2;
    while (*tok && (*tok)->type == WORDS)
    {
        ast->data = realloc(ast->data, sizeof(char *) * i);
        ast->data[i - 1] = (*tok)->value;

        if (print_steps)
        {
            printf("  Arg[%d]: %s\n", i - 1, ast->data[i - 1]);
        }
        free(*tok);
        *tok = lexer(input);
        i++;
    }
    ast->data = realloc(ast->data, sizeof(char *) * i);
    ast->data[i - 1] = NULL;
}

/*
static void creation_if(struct ast *ast, struct token **tok, FILE *input)
{
    ast->type = AST_IF;
    add_children(ast);
    struct token *tok = lexer(input);
    ast->children[0]->data = *tok;
    ast->children[0]->type = AST_CDT;
    *tok = lexer(input);
    add_children(ast);
    ast->children[1]->data = *tok;
    creation_then(ast->children[1]); //then
    int i = 2;
    while ((*tok)->type == ELIF)
    {
        add_children(ast);
        ast->children[i]->type = IF;
        ast->children[i]->data = tok;
        creation_if(ast->children[i],tok, input);
        i++;
    }
    if ((*tok)->type is ELSE)
    {
        add_children(ast);
        creation_then(ast->children[i]); //else
        i++;
    }
    add_children(ast); //next code
    creation_then(ast->children[i]);
}
*/

// Handle Creation of AST-Token
static void one_creation_ast(struct ast *ast, FILE* input, struct token **tok)
{
    if ((*tok)->type == WORDS)
    {
        struct ast *cmd_ast = ast_init();
        creation_command(cmd_ast, tok, input);
        add_children(ast, cmd_ast);
    }
    else if ((*tok)->type == IF)
    {
        struct ast *if_ast = ast_init();
        creation_if(if_ast, input, tok);
        add_children(ast, if_ast);
    }
    free(*tok);
    *tok = lexer(input);
}

// Function for IF condition
static void creation_if(struct ast *ast, FILE* input, struct token **tok)
{
    ast->type = AST_IF;

    struct ast *condition_ast = ast_init();
    condition_ast->type = AST_CONDITION;
    creation_command(condition_ast, tok, input);
    add_children(ast, condition_ast);
    *tok = lexer(input);

    if ((*tok)->type != THEN)
    {
        fprintf(stderr, "Error: Missing 'then'\n");
        exit(EXIT_FAILURE);
    }

    struct ast *then_ast = ast_init();
    then_ast->type = AST_THEN;
    *tok = lexer(input);
    while (*tok && (*tok)->type != ELSE && (*tok)->type != ELIF && (*tok)->type != FI)
    {
        creation_command(then_ast, tok, input);
        *tok = lexer(input);
    }
    add_children(ast, then_ast);

    // Handle ELIF
    while (*tok && (*tok)->type == ELIF)
    {
        struct ast *elif_ast = ast_init();
        elif_ast->type = AST_ELIF;
        *tok = lexer(input);
        creation_if(elif_ast, input, tok);
        add_children(ast, elif_ast);
    }

    // Handle ELSE
    if (*tok && (*tok)->type == ELSE)
    {
        struct ast *else_ast = ast_init();
        else_ast->type = AST_ELSE;
        *tok = lexer(input);
        while (*tok && (*tok)->type != FI)
        {
            creation_command(else_ast, tok, input);
            *tok = lexer(input);
        }
        add_children(ast, else_ast);
    }

    if (!*tok)
    {
        fprintf(stderr, "Error: unexpected end of input\n");
        exit(EXIT_FAILURE);
    }
    if ((*tok)->type != FI)
    {
        fprintf(stderr, "Error: Missing 'fi'\n");
        exit(EXIT_FAILURE);
    }
    *tok = lexer(input);
}

// Create AST
static void creation_ast(struct ast *ast, FILE* input)
{
    struct token *tok = lexer(input);
    while (tok && tok->type != END)
    {
        one_creation_ast(ast, input, &tok);
    }
    if (tok)
    {
        free(tok);
    }
}

// Main Function that call the Real MAIN
struct ast *parser(FILE* input)
{
    struct ast *ast_tok = ast_init();
    creation_ast(ast_tok, input);
    return ast_tok;
}

// Free the AST
void free_ast(struct ast *ast)
{
    for (size_t i = 0; i != ast->nb_children; i++)
    {
        free_ast(ast->children[i]);
    }
    free(ast->children);
    int j = 0;
    for (j = 0; ast->data[j]; j++)
    {	if (strcmp(ast->data[j], "0") != 0)
        	free(ast->data[j]);
    }
    free(ast->data[j]);
    free(ast->data);
    free(ast);
}

// Print the AST
void print_ast(struct ast *ast, int depth)
{
    if (!ast)
    {
        return;
    }
    for (int i = 0; i < depth; i++)
    {
        printf("  ");
    }
    printf("Node type: %d\n", ast->type);

    if (ast->data)
    {
        for (int i = 0; ast->data[i]; i++)
        {
            for (int j = 0; j < depth + 1; j++)
            {
                printf("  ");
            }
            printf("Data: %s\n", ast->data[i]);
        }
    }
    for (size_t i = 0; i < ast->nb_children; i++)
    {
        print_ast(ast->children[i], depth + 1);
    }
}
