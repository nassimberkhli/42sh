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
    
    if (print_steps)
        printf("[ast_init] AST node initialized with type: %d\n", ast_tok->type);
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

    if (print_steps)
    {
        printf("[creation_command] Finished creating command node\n");
    }
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

static void creation_pipeline(struct ast *ast, FILE *input, struct token **tok)
{
    if (print_steps)
    {
        printf("[creation_pipeline] Creating a pipeline node\n");
    }

    ast->type = AST_PIPELINE;

    struct token *current_tok = *tok;
    while (current_tok && current_tok->type != END)
    {
        struct ast *cmd_ast = ast_init();
        creation_command(cmd_ast, &current_tok, input);
        add_children(ast, cmd_ast);

        if (current_tok->type == PIPELINE)
        {
            free(current_tok);
            current_tok = lexer(input);
        }
        else
            break;
    }
    *tok = current_tok;

    if (print_steps)
    {
        printf("[creation_pipeline] Finished creating pipeline node\n");
    }
}

// Handle Creation of AST-Token
static void one_creation_ast(struct ast *ast, FILE *input, struct token **tok)
{
    if (!*tok || (*tok)->type == END)
    {
        if (print_steps)
            printf("[one_creation_ast] Reached END token, stopping\n");
        return;
    }

    if (print_steps)
    {
        printf("[one_creation_ast] Processing token: Type=%d, Value=%s\n", (*tok)->type, (*tok)->value);
    }

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
    else if ((*tok)->type == PIPELINE)
    {
        struct ast *pipeline_ast = ast_init();
        creation_pipeline(pipeline_ast, input, tok);
        add_children(ast, pipeline_ast);
    }

    free(*tok);
    *tok = lexer(input);
}

// Function for IF condition
static void creation_if(struct ast *ast, FILE* input, struct token **tok)
{
    if (print_steps)
    {
        printf("[creation_if] Starting 'if' condition creation\n");
    }

    ast->type = AST_IF;

    // Create and add the condition node
    struct ast *condition_ast = ast_init();
    condition_ast->type = AST_CONDITION;
    creation_command(condition_ast, tok, input);
    add_children(ast, condition_ast);

    if (print_steps)
    {
        printf("[creation_if] Condition node added\n");
    }

    *tok = lexer(input);

    // Check for 'then' token
    if ((*tok)->type != THEN)
    {
        fprintf(stderr, "Error: Missing 'then' after 'if'\n");
        exit(EXIT_FAILURE);
    }

    if (print_steps)
    {
        printf("[creation_if] 'then' token detected\n");
    }

    // Create and add the 'then' node
    struct ast *then_ast = ast_init();
    then_ast->type = AST_THEN;
    *tok = lexer(input);

    while (*tok && (*tok)->type != ELSE && (*tok)->type != ELIF && (*tok)->type != FI)
    {
        creation_command(then_ast, tok, input);
        *tok = lexer(input);
    }
    add_children(ast, then_ast);

    if (print_steps)
    {
        printf("[creation_if] 'then' block added\n");
    }

    // Handle 'elif' blocks
    while (*tok && (*tok)->type == ELIF)
    {
        if (print_steps)
        {
            printf("[creation_if] 'elif' token detected\n");
        }

        struct ast *elif_ast = ast_init();
        elif_ast->type = AST_ELIF;
        *tok = lexer(input);
        creation_if(elif_ast, input, tok);
        add_children(ast, elif_ast);

        if (print_steps)
        {
            printf("[creation_if] 'elif' block added\n");
        }
    }

    // Handle 'else' block
    if (*tok && (*tok)->type == ELSE)
    {
        if (print_steps)
        {
            printf("[creation_if] 'else' token detected\n");
        }

        struct ast *else_ast = ast_init();
        else_ast->type = AST_ELSE;
        *tok = lexer(input);

        while (*tok && (*tok)->type != FI)
        {
            creation_command(else_ast, tok, input);
            *tok = lexer(input);
        }
        add_children(ast, else_ast);

        if (print_steps)
        {
            printf("[creation_if] 'else' block added\n");
        }
    }

    // Ensure the 'fi' token is present
    if (!*tok)
    {
        fprintf(stderr, "Error: Unexpected end of input while parsing 'if'\n");
        exit(EXIT_FAILURE);
    }
    if ((*tok)->type != FI)
    {
        fprintf(stderr, "Error: Missing 'fi' to close 'if'\n");
        exit(EXIT_FAILURE);
    }

    if (print_steps)
    {
        printf("[creation_if] 'fi' token detected, completing 'if' block\n");
    }

    *tok = lexer(input);

    if (print_steps)
    {
        printf("[creation_if] Finished 'if' condition creation\n");
    }
}

// Create AST
static void creation_ast(struct ast *ast, FILE *input)
{
    if (print_steps)
    {
        printf("[creation_ast] Starting AST creation\n");
    }

    struct token *tok = lexer(input);
    while (tok && tok->type != END)
    {
        one_creation_ast(ast, input, &tok);
    }
    if (tok)
    {
        free(tok);
    }

    if (print_steps)
    {
        printf("[creation_ast] Finished AST creation\n");
    }
}

// Main Function that calls the Real MAIN
struct ast *parser(FILE *input)
{
    if (print_steps)
    {
        printf("[parser] Starting parsing process\n");
    }

    struct ast *ast_tok = ast_init();
    creation_ast(ast_tok, input);

    if (print_steps)
    {
        printf("[parser] Parsing process complete\n");
    }

    return ast_tok;
}

// Free the AST
void free_ast(struct ast *ast)
{
    if (!ast)
        return;

    if (print_steps)
    {
        printf("[free_ast] Freeing AST node of type: %d\n", ast->type);
    }

    for (size_t i = 0; i != ast->nb_children; i++)
    {
        if (ast->children[i])
            free_ast(ast->children[i]);
    }
    free(ast->children);

    int j = 0;
    while (ast->data && ast->data[j])
    {
        free(ast->data[j]);
        j++;
    }
    if (ast->data)
    {
        free(ast->data);
    }
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
