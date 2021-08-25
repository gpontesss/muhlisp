#ifndef EVAL_H_DECLARED
#define EVAL_H_DECLARED

#include "mpc.h"


enum {
    MUHLISP_VAL_ERR,
    MUHLISP_VAL_INT,
    MUHLISP_VAL_FLOAT,
};

typedef struct {
    int type;
    void* pval;
} muhlisp_val_t;

char* muhlisp_val_str(muhlisp_val_t* val);
void muhlisp_val_free(muhlisp_val_t* val);

void eval_ast(mpc_ast_t* ast, muhlisp_val_t* val);
void eval_expr(mpc_ast_t* ast, muhlisp_val_t* val);
void eval_number(mpc_ast_t* ast, muhlisp_val_t* val);

#endif
