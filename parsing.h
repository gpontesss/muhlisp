#ifndef PARSING_H_INCLUDED
#define PARSING_H_INCLUDED

#include "./mpc.h"
#include "./val.h"

typedef struct {
    mpc_parser_t* Number;
    mpc_parser_t* Symbol;
    mpc_parser_t* Expr;
    mpc_parser_t* SExpr;
    mpc_parser_t* MuhLisp;
} muhlisp_parser_t;

mpc_err_t* muhlisp_init_parser(muhlisp_parser_t*);
void muhlisp_free_parser(muhlisp_parser_t*);

void muhlisp_parser_read_val(const muhlisp_parser_t* parser, const char* input,
        muhlisp_val_t* val);

void muhlisp_read_val(mpc_ast_t* ast, muhlisp_val_t* val);
void muhlisp_read_val_expr(mpc_ast_t* ast, muhlisp_val_t* val);
void muhlisp_read_val_sexpr(mpc_ast_t* ast, muhlisp_val_t* val);
void muhlisp_read_val_number(mpc_ast_t* ast, muhlisp_val_t* val);
void muhlisp_read_val_symbol(mpc_ast_t* ast, muhlisp_val_t* val);

#endif
