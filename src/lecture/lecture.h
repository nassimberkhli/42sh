#ifndef LECTURE_H
#define LECTURE_H

#include <stdio.h>
#include <unistd.h>

#include "../parser/parser.h"

int exec(struct ast *ast);
void exec_pipeline(struct ast *pipeline_ast);
int exec_command(char **data);
int lecture(FILE *input);
void exec_redir_out(struct ast *ast, int mode);
void exec_redir_in(struct ast *ast);

#endif /* LECTURE_H */
