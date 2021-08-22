#ifndef PARSING_H_INCLUDED
#define PARSING_H_INCLUDED

#include "mpc.h"

typedef struct {
    mpc_parser_t* Number;
    mpc_parser_t* Operator;
    mpc_parser_t* Expr;
    mpc_parser_t* MuhLisp;
} muhlisp_parser_t;

muhlisp_parser_t muhlisp_create_parser();
void muhlisp_free_parser(muhlisp_parser_t parser);

int muhlisp_parse_input( \
        const muhlisp_parser_t* parser, \
        const char* input, mpc_result_t* result);

#endif
