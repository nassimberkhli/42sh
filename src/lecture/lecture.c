#include "lecture.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../variable/variable.h"
#include "../builtin/echo.h"

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
    else if (ast->type == AST_PIPELINE)
        exec_pipeline(ast);
}

void exec_pipeline(struct ast *pipeline_ast)
{
    if (!pipeline_ast || pipeline_ast->type != AST_PIPELINE)
        return;

    int pipefd[2];
    int prev_fd = -1;
    pid_t pid;

    for (size_t i = 0; i < pipeline_ast->nb_children; i++)
    {
        if (i < pipeline_ast->nb_children - 1)
        {
            if (pipe(pipefd) == -1)
                exit(EXIT_FAILURE);
        }

        pid = fork();
        if (pid == -1)
            exit(EXIT_FAILURE);

        if (pid == 0)
        {
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            if (i < pipeline_ast->nb_children - 1)
            {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }

            close(pipefd[0]);
            exec(pipeline_ast->children[i]);
            exit(EXIT_FAILURE);
        }
        else
        {
            waitpid(pid, NULL, 0);

            if (prev_fd != -1)
                close(prev_fd);

            if (i < pipeline_ast->nb_children - 1)
            {
                close(pipefd[1]);
                prev_fd = pipefd[0];
            }
        }
    }
}

void exec_command(char **data)
{
     if (!data || !*data)
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
