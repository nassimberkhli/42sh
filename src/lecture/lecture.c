#include "lecture.h"
#include "../parser/parser.h"
#include <sys/wait.h>
#include <stdlib.h>

void exec_command(char **data)
{
    if (data && data[0])
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            return;
        }
        if (pid == 0)
        {
            execvp(data[0], data);
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);
        }
    }
}




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



struct ast *lecture(FILE* input)
{
    struct ast *ast = parser(input);
    exec(ast);
    return ast;
}