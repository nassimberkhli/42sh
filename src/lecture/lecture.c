#include "lecture.h"

#include <stddef.h>
#include <string.h>

#include "../parser/parser.h"
#include "../variable/variable.h"
#include "../builtin/echo.h"
#include <sys/wait.h>
#include <stdlib.h>


extern int print_steps;


/*
void exec_command(char **data)
{
    if (print_steps)
    {
        printf("[exec_command] Executing command:\n");
        for (int i = 0; data && data[i]; i++)
        {
            printf("  Arg[%d]: %s\n", i, data[i]);
        }
    }

    if (data && data[0])
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            if (print_steps)
                printf("[exec_command] Fork failed\n");
            return;
        }
        if (pid == 0)
        {
            if (print_steps)
                printf("[exec_command] Executing execvp\n");
            execvp(data[0], data);
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);
            if (print_steps)
                printf("[exec_command] Command exited with status: %d\n", status);
        }
    }
}
*/
void exec_command(char **data)
{
	if (*data == NULL)
		return;
	if (strcmp(*data, "echo") == 0)
	{
		echo(data);
		return;
	}
	for (size_t i = 0; i < strlen(data[0]) - 1; i++)
	{
		if (data[0][i] == '=')
		{
			variable(data);
			return;
		}
	}
}

void exec(struct ast *ast)
{
    size_t i = 0;

    if (print_steps)
    {
        printf("[exec] Executing AST node:\n");
        printf("  Node type: %d\n", ast->type);
        if (ast->data)
        {
            for (int j = 0; ast->data[j]; j++)
                printf("  Data[%d]: %s\n", j, ast->data[j]);
        }
    }

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

struct ast *lecture(FILE* input)
{
    struct ast *ast = parser(input);

    if (print_steps)
    {
        printf("Parsed AST:\n");
        print_ast(ast, 0);
    }

    exec(ast);
    return ast;
}
