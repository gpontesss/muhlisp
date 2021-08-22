#include "mpc.h"
#include "parsing.h"

/*
    Polish expressions:
    + 1 2 6
    + 6 (* 2 9)
    / (* 10 2) (+ 4 2)

    Polish grammar:
        number: /-?[0-9]+/ ;
        operator: '+' | '-' | '*' | '/' ;
        expr: <number> \
            | '(' <operator> <expr>+ ')' ;
*/

mpc_err_t* muhlisp_init_parser(muhlisp_parser_t* parser) {
    *parser = (muhlisp_parser_t){
        .Number = mpc_new("number"),
        .Operator = mpc_new("operator"),
        .Expr = mpc_new("expr"),
        .MuhLisp = mpc_new("muhlisp"),
    };

    return mpca_lang(MPCA_LANG_DEFAULT,
        " \
            number: /-?[0-9]+/ ; \
            operator: '+' | '-' | '*' | '/' ; \
            expr: <number> | '(' <operator> <expr>+ ')' ; \
            muhlisp: /^/ <expr> /$/ ; \
        ",
        parser->Number, parser->Operator,
        parser->Expr, parser->MuhLisp
    );
}

void muhlisp_free_parser(muhlisp_parser_t* parser) {
    mpc_cleanup(4, \
            parser->Number, parser->Operator, \
            parser->Expr, parser->MuhLisp);
}

int muhlisp_parse_input( \
        const muhlisp_parser_t* parser, \
        const char* input, mpc_result_t* result) {
    return mpc_parse("<stdin>", input, parser->MuhLisp, result);
}
