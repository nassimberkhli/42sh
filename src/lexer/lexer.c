#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_DICO 18

extern int print_steps;

// init token
static struct token *init_tok()
{
    struct token *tok = malloc(sizeof(struct token));
    tok->value = NULL;
    return tok;
}

// create word by combine characters
static char *create_word(char *word, char c)
{
    int size = strlen(word) + 1;
    word = realloc(word, size + 1);
    word[size - 1] = c;
    word[size] = '\0';
    return word;
}
// create token
struct token *create_tok(char *word)
{
    struct token *tok = init_tok();

    struct
    {
        char *value;
        enum token_type type;
    } dicoToken[] = { { "if", IF },       { "fi", FI },      { "then", THEN },
                      { "elif", ELIF },   { "else", ELSE },  { ";", COMMA },
                      { "\n", RET_LINE }, { "for", FOR },    { "while", WHILE },
                      { "until", UNTIL }, { "do", DO },      { "done", DONE },
                      { "'", QUOTE },     { ">", REDIR },    { ">|", REDIR },
                      { ">>", APP },      { "|", PIPELINE }, { "EOF", END } };

    for (int i = 0; i != SIZE_DICO; i++)
    {
        // compare if word equal to a no WORD token
        if (!strcmp(word, dicoToken[i].value))
        {
            tok->type = dicoToken[i].type;
            tok->value = dicoToken[i].value;
            if (dicoToken[i].type != END)
                free(word);
            return tok;
        }
    }

    tok->type = WORDS;
    tok->value = word;
    return tok;
}

// main lex function
struct token *lexer(FILE *input)
{
    if (print_steps)
        printf("[lexer] Starting lexical analysis\n");

    int c = fgetc(input);
    char *word = calloc(1, sizeof(char) * 1);
    while (c == ' ')
        c = fgetc(input);

    if (c == EOF)
    {
        free(word);
        if (print_steps)
            printf("[lexer] End of file reached\n");
        return create_tok("EOF");
    }
    while (c != EOF)
    {
		if (c == '\\')
		{
			c = fgetc(input);
            word = create_word(word, c);
		}
        else if (c == '#')
        {
            while (c != '\n' && c != EOF)
            {
                c = fgetc(input);
            }
            if (strlen(word) > 0)
                fseek(input, -1, SEEK_CUR);
            break;
        }
        else if (c == '\'')
        {
            while (c != '\'' && c != EOF)
            {
                word = create_word(word, c);
                c = fgetc(input);
            }
        }
        else if (c != ' ' && c != '\n' && c != ';' && c != '>')
            word = create_word(word, c);
        else
        {
            if (strlen(word) > 0)
                fseek(input, -1, SEEK_CUR);
            break;
        }
        c = fgetc(input);
    }

    if (strlen(word) <= 0 && c != ' ' && c != EOF)
        word = create_word(word, c);

    struct token *tok = create_tok(word);

    if (print_steps)
    {
        printf("[lexer] Token created:\n");
        printf("  Type: %d\n", tok->type);
        printf("  Value: %s\n", tok->value);
    }

    return tok;
}
