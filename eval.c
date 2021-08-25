#define _GNU_SOURCE  // Required for vasprintf.

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "mpc.h"
#include "eval.h"


static muhlisp_val_t muhlisp_val_double(double val) {
    double* pval = (double*)malloc(sizeof(double));
    *pval = val;
    return (muhlisp_val_t){
        .type = MUHLISP_VAL_FLOAT,
        .pval = pval,
    };
}

static muhlisp_val_t muhlisp_val_errorf(char* fmt, ...) {
    char* errstr;
    va_list args;

    va_start(args, fmt);
    // TODO: handle errors
    vasprintf(&errstr, fmt, args);
    va_end(args);

    return (muhlisp_val_t){
        .type = MUHLISP_VAL_ERR,
        .pval = errstr,
    };
}

char* muhlisp_val_str(muhlisp_val_t* val) {
    char* str;
    switch(val->type) {
        case MUHLISP_VAL_ERR:
            return val->pval;
        case MUHLISP_VAL_FLOAT:
            asprintf(&str, "%f", *((double*)val->pval));
            return str;
        default:
            return NULL;
    }
}

void muhlisp_val_free(muhlisp_val_t* val) {
    // TODO: in some cases where pval points to static memory it may not
    // work (?)
    free(val->pval);
}

void eval_ast(mpc_ast_t *ast, muhlisp_val_t *val) {
    if (strstr(ast->tag, "expr") != NULL) {
        return eval_expr(ast, val);
    // root expression
    } else if(strcmp(ast->tag, ">") == 0) {
        // selects second child; it should be an expression.
        return eval_ast(ast->children[1], val);
    } else {
        *val = muhlisp_val_errorf("Cannot parse '%s' rule kind", ast->tag);
    }
}

void eval_expr(mpc_ast_t *ast, muhlisp_val_t *val) {
    // TODO: should expression AST be validated?
    if (strstr(ast->tag, "number") != NULL) {
        return eval_number(ast, val);
    }

    mpc_ast_t** children = ast->children;
    muhlisp_val_t left_val, right_val;

    eval_expr(children[2], &left_val);
    if(left_val.type == MUHLISP_VAL_ERR) {
        *val = left_val;
        return;
    }
    eval_expr(children[3], &right_val);
    if(right_val.type == MUHLISP_VAL_ERR) {
        *val = right_val;
        return;
    }

    // TODO: only evaluating numbers for now
    double left  = *((double*)(left_val.pval));
    double right = *((double*)(right_val.pval));

    muhlisp_val_free(&left_val);
    muhlisp_val_free(&right_val);

    // operators only have one character
    char op = children[1]->contents[0];

    // TODO: maybe some routing to basic operator predicates to each types?
    // don't know enough of lisp yet to know what to do.
    switch (op) {
    case '+':
        *val = muhlisp_val_double(left + right);
        break;
    case '-':
        *val = muhlisp_val_double(left - right);
        break;
    case '*':
        *val = muhlisp_val_double(left * right);
        break;
    case '/':
        if( right == 0 ) {
            *val = muhlisp_val_errorf("Cannot divide by 0, you fool");
        } else {
            *val = muhlisp_val_double(left / right);
        }
        break;
    case '^':
            *val = muhlisp_val_double(pow(left, right));
        break;
    default:
        // operation is not recognized
        *val = muhlisp_val_errorf("Operation '%c' cannot be evaluated.", op);
    }
}

void eval_number(mpc_ast_t *ast, muhlisp_val_t *val) {
    // TODO: only evaluating decimal numbers for now.
    // TODO: bad atof does not detect errors :(
    *val = muhlisp_val_double(atof(ast->contents));
}
