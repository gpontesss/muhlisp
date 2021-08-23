#ifndef EVAL_H_DECLARED
#define EVAL_H_DECLARED

#include "mpc.h"

int eval_ast(mpc_ast_t* ast, void* value);
int eval_expr(mpc_ast_t* ast, void* value);
int eval_number(mpc_ast_t* ast, void* value);

#endif
