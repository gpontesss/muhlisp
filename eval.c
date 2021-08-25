#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "mpc.h"
#include "eval.h"

// TODO: how to deal with it properly?
#define PSPRINTF_STR_SIZE 4098


static char* psprintf(const char* fmt, ...) {
    char* errstr = (char*)malloc(sizeof(char)*PSPRINTF_STR_SIZE);

    va_list args;
    va_start(args, fmt);
    vsprintf(errstr, fmt, args);
    va_end(args);

    return errstr;
}

char* muhlisp_value_str(muhlisp_val_t* val) {
    switch(val->type) {
        case MUHLISP_VAL_ERR:
            return val->pval;
            break;
        case MUHLISP_VAL_FLOAT:
            return psprintf("%f", *((double*)val->pval));
        default:
            return NULL;
    }
}

void eval_ast(mpc_ast_t *ast, muhlisp_val_t *val) {
    if (strstr(ast->tag, "expr") != NULL) {
        return eval_expr(ast, val);
    // root expression
    } else if(strcmp(ast->tag, ">") == 0) {
        // selects second child; it should be an expression.
        return eval_ast(ast->children[1], val);
    } else {
        *val = (muhlisp_val_t){
            .type = MUHLISP_VAL_ERR,
            .pval = psprintf("Cannot parse '%s' rule kind.", ast->tag),
        };
    }
}

void eval_expr(mpc_ast_t *ast, muhlisp_val_t *val) {
    // TODO: should expression AST be validated?
    if (strstr(ast->tag, "number") != NULL) {
        return eval_number(ast, val);
    }

    mpc_ast_t **children = ast->children;
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

    // TODO: check values
    // operators only have one character
    char op = children[1]->contents[0];

    double* pval = (double*)malloc(sizeof(double));
    switch (op) {
    case '+':
        *pval = left + right;
        *val = (muhlisp_val_t){ .type = MUHLISP_VAL_FLOAT, .pval = pval };
        break;
    case '-':
        *pval = left - right;
        *val = (muhlisp_val_t){ .type = MUHLISP_VAL_FLOAT, .pval = pval };
        break;
    case '*':
        *pval = left * right;
        *val = (muhlisp_val_t){ .type = MUHLISP_VAL_FLOAT, .pval = pval };
        break;
    case '/':
        if( right == 0 ) {
            *val = (muhlisp_val_t){
                .type = MUHLISP_VAL_ERR,
                .pval = "Cannot divide by 0, you fool",
            };
        } else {
            *pval = left / right;
            *val = (muhlisp_val_t){ .type = MUHLISP_VAL_FLOAT, .pval = pval };
        }
        break;
    case '^':
        *pval = pow(left, right);
        *val = (muhlisp_val_t){ .type = MUHLISP_VAL_FLOAT, .pval = pval };
        break;
    default:
        // operation is not recognized
        *val = (muhlisp_val_t){
            .type = MUHLISP_VAL_ERR,
            .pval = psprintf("Operation '%c' cannot be evaluated.", op),
        };
    }
}

void eval_number(mpc_ast_t *ast, muhlisp_val_t *val) {
    // TODO: only evaluating decimal numbers for now.
    double* pval = (double*)malloc(sizeof(double));
    // TODO: bad atof does not detect errors :(
    *pval = atof(ast->contents);

    *val = (muhlisp_val_t){
        .type = MUHLISP_VAL_FLOAT,
        .pval = pval,
    };
}
