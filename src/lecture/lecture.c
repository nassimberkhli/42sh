#include "lecture.h"

#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../builtin/echo.h"
#include "../variable/variable.h"

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
                printf("[exec_command] Command exited with status: %d\n",
status);
        }
    }
}
*/

int exec(struct ast *ast)
{
    size_t i = 0;
    int value = 0;
    int val = 0;
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

    if (ast->type == AST_CMD && ast->children[0]
        && ast->children[0]->type == AST_REDIR_OUT)
    {
        exec_redir_out(ast, 0);
    }
    else if (ast->type == AST_CMD && ast->children[0]
        && ast->children[0]->type == AST_REDIR_OUT_APP)
    {
        exec_redir_out(ast, 1);
    }
    else if (ast->type == AST_CMD && ast->children[0]
        && ast->children[0]->type == AST_REDIR_IN)
    {
        exec_redir_in(ast);
    }
    else if (ast->type == AST_CMD)
    {
        val = exec_command(ast->data);
        if (val != 0)
        {
            value = val;
        }
        else
        {
            while (i < ast->nb_children)
            {
                val = exec(ast->children[i]);
                if (val != 0)
                {
                    value = val;
                    break;
                }
                i++;
            }
        }
    }
    else if (ast->type == AST_PIPELINE)
        exec_pipeline(ast);
    return value;
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

void exec_redir_out(struct ast *ast, int mode)
{
    char *file = ast->children[0]->data[0];
    int fd = 0;
    int saved_stdout = dup(STDOUT_FILENO);
    if (mode == 0)
        fd = open(file, O_CREAT | O_WRONLY, 0644);
    else if (mode == 1)
        fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
    dup2(fd, STDOUT_FILENO);
    exec_command(ast->data);
    fflush(stdout);
    dup2(saved_stdout, STDOUT_FILENO);
    close(fd);
}

void exec_redir_in(struct ast *ast)
{
    char *file = ast->children[0]->data[0];
    int fd = 0;
    int saved_stdin = dup(0);
    fd = open(file,O_RDONLY);
    dup2(fd, 0);
    close(fd);
    exec_command(ast->data);
    dup2(saved_stdin, 0);
    close(saved_stdin);
}

int exec_command(char **data)
{
    int value = 0;
    if (!data || !*data)
        return 0;

    if (strcmp(*data, "echo") == 0)
    {
        echo(data);
        return 0;
    }
    for (size_t i = 0; i < strlen(data[0]) - 1; i++)
    {
        if (data[0][i] == '=')
        {
            variable(data);
            return 0;
        }
    }
    pid_t pid;
    pid = fork();
    if (pid == -1)
        exit(EXIT_FAILURE);
    else if (pid == 0)
    {
        execvp(data[0], data);
    }
    else // On est dans le parent
    {
        int status;
        waitpid(pid, &status, 0);
        return value;
    }
}

int lecture(FILE *input)
{
    struct ast *ast = parser(input);

    if (print_steps)
    {
        printf("Parsed AST:\n");
        print_ast(ast, 0);
    }
    if (!ast)
        return 2;

    int value = exec(ast);
    free_ast(ast);
    return value;
}
