#include "lecture.h"
#include "../parser/parser.h"
#include "../builtin/echo.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

//execute command of then ast
void exec_command(char **data)
{
	if (strcmp(data[0], "echo") == 0)
		echo(data);
}

//read ast for execute it
void exec(struct ast *ast)
{
    size_t i = 0;
    if (ast->type == AST_CMD)
    {
        exec_command(ast->data);
        while (i < ast->nb_children)
        {
            exec(ast->children[i]);
            i++;
        }
    }
}

//??????????
struct ast *lecture(FILE* input)
{
    struct ast *ast = parser(input);
    exec(ast);
    return ast;
}
