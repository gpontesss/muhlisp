#include "./mpc.h"
#include "./parsing.h"
#include "./val.h"

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

int muhlisp_parse_input(const muhlisp_parser_t* parser, const char* input,
        mpc_result_t* result) {
    return mpc_parse("<stdin>", input, parser->MuhLisp, result);
}

void muhlisp_read_val(mpc_ast_t *ast, muhlisp_val_t *val) {
    // root expression
    if(strcmp(ast->tag, ">") == 0) {
        // selects second child; it should be an expression.
        muhlisp_read_val(ast->children[1], val);
    // sexpr match has to come before expr's; if not, bugs'll hunt you.
    } else if(strstr(ast->tag, "sexpr") != NULL) {
        muhlisp_read_val_sexpr(ast, val);
    } else if(strstr(ast->tag, "expr") != NULL) {
        muhlisp_read_val_expr(ast, val);
    } else {
        *val = muhlisp_val_errorf("Cannot parse '%s' rule kind", ast->tag);
    }
}

void muhlisp_read_val_sexpr(mpc_ast_t* ast, muhlisp_val_t* val) {
    // sexpr = '(' <expr>* ')'
   int children_num = ast->children_num;
   mpc_ast_t** children = ast->children;

   muhlisp_val_t sexpr = muhlisp_val_sexpr(children_num-2);
   muhlisp_sexpr_list_t* sexpr_list = (muhlisp_sexpr_list_t*)sexpr.pval;

   for(int i = 0; i < children_num-2; i++) {
        mpc_ast_t* child = children[i+1];
        muhlisp_val_t child_val;
        muhlisp_read_val_expr(child, &child_val);
        sexpr_list->pvals[i] = muhlisp_val_ptr(child_val);
   }

   *val = sexpr;
}

void muhlisp_read_val_expr(mpc_ast_t *ast, muhlisp_val_t *val) {
    // TODO: should expression AST be validated?
    if(strstr(ast->tag, "sexpr") != NULL) {
        muhlisp_read_val_sexpr(ast, val);
    } else if(strstr(ast->tag, "symbol") != NULL) {
        muhlisp_read_val_symbol(ast, val);
    } else if(strstr(ast->tag, "number") != NULL) {
        muhlisp_read_val_number(ast, val);
    } else {
        *val = muhlisp_val_errorf(
            "Can't eval expression '%s' (unhandled tag '%s')",
            ast->contents,
            ast->tag
        );
    }
}

void muhlisp_read_val_symbol(mpc_ast_t* ast, muhlisp_val_t* val) {
    // symbol = '+' | '-' | '*' | '/' | '^'
    *val = muhlisp_val_symbol(ast->contents);
}

void muhlisp_read_val_number(mpc_ast_t *ast, muhlisp_val_t *val) {
    // TODO: bad atof does not detect errors :(
    *val = muhlisp_val_double(atof(ast->contents));
}
