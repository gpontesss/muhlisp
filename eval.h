#ifndef EVAL_H_DECLARED
#define EVAL_H_DECLARED

#include "mpc.h"


enum {
    MUHLISP_VAL_ERR,
    MUHLISP_VAL_NUMBER,
    MUHLISP_VAL_SYM,
    MUHLISP_VAL_SEXPR,
};

typedef struct {
    int type;
    void* pval;
} muhlisp_val_t;

typedef struct {
    int count;
    muhlisp_val_t** pvals;
} muhlisp_sexpr_list_t;

char* muhlisp_val_str(muhlisp_val_t* val);
void muhlisp_val_free(muhlisp_val_t* val);

void eval_ast(mpc_ast_t* ast, muhlisp_val_t* val);
void eval_expr(mpc_ast_t* ast, muhlisp_val_t* val);
void eval_sexpr(mpc_ast_t* ast, muhlisp_val_t* val);
void eval_number(mpc_ast_t* ast, muhlisp_val_t* val);
void eval_symbol(mpc_ast_t* ast, muhlisp_val_t* val);

#endif
