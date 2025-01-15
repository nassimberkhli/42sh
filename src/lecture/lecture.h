#ifndef LECTURE_H
#define LECTURE_H

#include <unistd.h>
#include <stdio.h>

#include "../parser/parser.h"

void exec(struct ast *ast);
void exec_pipeline(struct ast *pipeline_ast);
void exec_command(char **data);
struct ast *lecture(FILE* input);

#endif /* LECTURE_H */
