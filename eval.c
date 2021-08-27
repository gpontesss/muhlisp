#define _GNU_SOURCE  // Required for vasprintf.

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "mpc.h"
#include "eval.h"


static muhlisp_val_t* muhlisp_val_ptr(muhlisp_val_t val) {
    // TODO: proper error checking/signaling
    muhlisp_val_t* pval = (muhlisp_val_t*)malloc(sizeof(muhlisp_val_t));
    *pval = val;
    return pval;
}

static muhlisp_val_t muhlisp_val_double(double val) {
    double* pval = (double*)malloc(sizeof(double));
    *pval = val;
    return (muhlisp_val_t){
        .type = MUHLISP_VAL_NUMBER,
        .pval = pval,
    };
}

static muhlisp_val_t muhlisp_val_errorf(char* fmt, ...) {
    char* errstr;
    va_list args;

    va_start(args, fmt);
    // strcpy is not needed, for vasprintf creates a new string. thus it also
    // solves the problem of trying to free static memory.
    // TODO: handle errors
    vasprintf(&errstr, fmt, args);
    va_end(args);

    return (muhlisp_val_t){
        .type = MUHLISP_VAL_ERR,
        .pval = errstr,
    };
}

static muhlisp_val_t muhlisp_val_sexpr(int count) {
    // TODO: handle errors
    muhlisp_val_t** sexpr_pvals = malloc(count*sizeof(muhlisp_val_t*));
    muhlisp_sexpr_list_t* sexpr_list = malloc(sizeof(muhlisp_sexpr_list_t));

    *sexpr_list = (muhlisp_sexpr_list_t){
        .count = count,
        .pvals = sexpr_pvals,
    };

    return (muhlisp_val_t){
        .type = MUHLISP_VAL_SEXPR,
        .pval = sexpr_list,
    };
}

static muhlisp_val_t muhlisp_val_symbol(const char* symbol) {
    size_t len = strlen(symbol) + 1;
    // TODO: handle errors.
    char* symbolcpy = malloc(len*(sizeof(char)));
    strncpy(symbolcpy, symbol, len);

    return (muhlisp_val_t){
        .type = MUHLISP_VAL_SYM,
        .pval = symbolcpy,
    };
}

char* muhlisp_val_str(muhlisp_val_t* val) {
    int type = val->type;

    if(type == MUHLISP_VAL_ERR || type == MUHLISP_VAL_SYM) {
        return val->pval;
    } else if(type == MUHLISP_VAL_NUMBER) {
        char* str;
        asprintf(&str, "%f", *((double*)val->pval));
        return str;
    } else if(type == MUHLISP_VAL_SEXPR) {
        muhlisp_sexpr_list_t* sexpr_list = val->pval;
        char* str = "(";
        for(int i = 0; i < sexpr_list->count; i++) {
            asprintf(&str, "%s %s", str, muhlisp_val_str(sexpr_list->pvals[i]));
        }
        asprintf(&str, "%s )", str);
        return str;
    } else {
        return NULL;
    }
}

void muhlisp_val_free(muhlisp_val_t* pval) {
    if(pval->type == MUHLISP_VAL_SEXPR) {
        muhlisp_sexpr_list_t* sexpr = pval->pval;
        for(int i = 0; i < sexpr->count; i++) {
            muhlisp_val_free(sexpr->pvals[i]);
        }
        free(sexpr->pvals);
    }
    // maybe some problems may occur if pval->pval points to static memory. try
    // to avoid that.
    free(pval->pval);
    // TODO: should try to free pval itself? there are cases where it is a
    // struct in the stack.
}

void eval_ast(mpc_ast_t *ast, muhlisp_val_t *val) {
    // root expression
    if(strcmp(ast->tag, ">") == 0) {
        // selects second child; it should be an expression.
        eval_ast(ast->children[1], val);
    // sexpr match has to come before expr's; if not, bugs'll hunt you.
    } else if(strstr(ast->tag, "sexpr") != NULL) {
        eval_sexpr(ast, val);
    } else if(strstr(ast->tag, "expr") != NULL) {
        eval_expr(ast, val);
    } else {
        *val = muhlisp_val_errorf("Cannot parse '%s' rule kind", ast->tag);
    }
}

void eval_sexpr(mpc_ast_t* ast, muhlisp_val_t* val) {
    // sexpr = '(' <expr>* ')'
   int children_num = ast->children_num;
   mpc_ast_t** children = ast->children;

   muhlisp_val_t sexpr = muhlisp_val_sexpr(children_num-2);
   muhlisp_sexpr_list_t* sexpr_list = (muhlisp_sexpr_list_t*)sexpr.pval;

   for(int i = 0; i < children_num-2; i++) {
        mpc_ast_t* child = children[i+1];
        muhlisp_val_t child_val;
        // TODO: how to deal with errors mid-child-evaluation?
        eval_expr(child, &child_val);
        sexpr_list->pvals[i] = muhlisp_val_ptr(child_val);
   }

   *val = sexpr;
}

void eval_expr(mpc_ast_t *ast, muhlisp_val_t *val) {
    // TODO: should expression AST be validated?
    if(strstr(ast->tag, "sexpr") != NULL) {
        eval_sexpr(ast, val);
    } else if(strstr(ast->tag, "symbol") != NULL) {
        eval_symbol(ast, val);
    } else if(strstr(ast->tag, "number") != NULL) {
        eval_number(ast, val);
    } else {
        *val = muhlisp_val_errorf(
            "Can't eval expression '%s' (unhandled tag '%s')",
            ast->contents,
            ast->tag
        );
    }
}

void eval_symbol(mpc_ast_t* ast, muhlisp_val_t* val) {
    // symbol = '+' | '-' | '*' | '/' | '^'
    *val = muhlisp_val_symbol(ast->contents);
}

void eval_number(mpc_ast_t *ast, muhlisp_val_t *val) {
    // TODO: bad atof does not detect errors :(
    *val = muhlisp_val_double(atof(ast->contents));
}
