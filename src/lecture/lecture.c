#include "lecture.h"

#include <stddef.h>

#include "../parser/parser.h"

void exec_command(char **data)
{
    if (data && data[0])
        execvp(data[0],data);
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
