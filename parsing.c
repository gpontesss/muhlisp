#include "mpc.h"
#include "parsing.h"

mpc_err_t* muhlisp_init_parser(muhlisp_parser_t* parser) {
    *parser = (muhlisp_parser_t){
        .Number = mpc_new("number"),
        .Symbol = mpc_new("symbol"),
        .Expr = mpc_new("expr"),
        .SExpr = mpc_new("sexpr"),
        .MuhLisp = mpc_new("muhlisp"),
    };

    return mpca_lang(MPCA_LANG_DEFAULT,
        " \
            number: /-?[0-9]+(\\.[0-9]+)?/ ; \
            symbol: '+' | '-' | '*' | '/' | '^' ; \
            sexpr: '(' <expr>* ')' ; \
            expr: <number> | <symbol> | <sexpr> ; \
            muhlisp: /^/ <sexpr> /$/ ; \
        ",
        parser->Number, parser->Symbol, parser->SExpr,
        parser->Expr,   parser->MuhLisp
    );
}

void muhlisp_free_parser(muhlisp_parser_t* parser) {
    mpc_cleanup(5, \
            parser->Number, parser->Symbol, parser->SExpr, \
            parser->Expr, parser->MuhLisp);
}

int muhlisp_parse_input( \
        const muhlisp_parser_t* parser, \
        const char* input, mpc_result_t* result) {
    return mpc_parse("<stdin>", input, parser->MuhLisp, result);
}
