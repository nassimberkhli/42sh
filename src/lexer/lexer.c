#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

#define SIZE_DICO 16
// init token
static struct token *init_tok()
{
    struct token *tok = malloc(sizeof(struct token));
    tok->value = NULL;
    return tok;
} 


// create word by combine characters
static char* create_word(char *word, char c)
{
    int size = strlen(word) + 1;
    word = realloc(word, size + 1);
    word[size- 1] = c;
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
        } dicoToken[] = {{"if", IF}, {"fi", FI}, {"then", THEN}, {"elif", ELIF}, {"else", ELSE}, {";", COMMA}, {"\n", RET_LINE}, {"for", FOR}, {"while", WHILE}, {"until", UNTIL}, {"do", DO}, {"done", DONE}, {"'", QUOTE}, {">", REDIR}, {">>", APP}, {"EOF", END}};
    
    for (int i = 0 ; i!= SIZE_DICO;i++)
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


//main lex function
struct token *lexer(FILE *input)
{
    char c = fgetc(input);
    char *word = calloc(1, sizeof(char) * 1);
    while (c == ' ')
        c = fgetc(input);
    if (c == EOF)
    {
        free(word);
        return create_tok("EOF");
    }
    //definition
    while (c != EOF)
    {
        if (c != ' ' && c != '\n' && c != ';')
            word = create_word(word, c);
        else
        {
            if (strlen(word) > 0)
                fseek(input, -1, SEEK_CUR);
            break;
        }
        c = fgetc(input);
    }
    
    if (strlen(word) <= 0 && c != ' ')
        word = create_word(word, c);
    
    struct token *tok = create_tok(word);
    return tok;
}
