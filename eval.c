#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "mpc.h"
#include "eval.h"

int eval_ast(mpc_ast_t* ast, void* value) {
    if(strstr(ast->tag, "expr") != NULL) {
        return eval_expr(ast, value);
    // root expression
    } else if(!strcmp(ast->tag, ">")) {
        // selects second child; it should be an expression.
        return eval_ast(ast->children[1], value);
    } else {
        // don't know how to parse it
        return -1;
    }
}

int eval_expr(mpc_ast_t* ast, void* value) {
    // should expression AST be validated?
    if(!strcmp(ast->tag, "expr|number|regex")) {
        return eval_number(ast, value);
    }

    mpc_ast_t** children = ast->children;

    // only evaluating numbers for now
    double left, right;
    // these can't error for now
    eval_expr(children[2], &left);
    eval_expr(children[3], &right);

    // operators only have one character
    char op = children[1]->contents[0];
    switch(op) {
        case '+':
            *((double*)value) = left + right;
            break;
        case '-':
            *((double*)value) = left - right;
            break;
        case '*':
            *((double*)value) = left * right;
            break;
        case '/':
            *((double*)value) = left / right;
            break;
        default:
            // operation is not recognized
            return -2;
    }
    return 0;
}

int eval_number(mpc_ast_t* ast, void* value) {
    // TODO: bad atof does not detect errors :(
    *(double*)value = atof(ast->contents);
    return 0;
}
