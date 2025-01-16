#ifndef LECTURE_H
#define LECTURE_H

#include <stdio.h>
#include <unistd.h>

#include "../parser/parser.h"

void exec(struct ast *ast);
void exec_pipeline(struct ast *pipeline_ast);
void exec_command(char **data);
struct ast *lecture(FILE *input);
void exec_redir(struct ast *ast, int mode);

#endif /* LECTURE_H */
