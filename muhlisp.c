#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>
#include <string.h>

#include "mpc.h"
#include "parsing.h"

#include "eval.h"


int main(int argc, char** argv) {
    puts("muhlisp version 0.0.1");
    puts("press ctrl+c to quit");

    muhlisp_parser_t parser;
    mpc_err_t* err = muhlisp_init_parser(&parser);

    if(err) {
        mpc_err_print(err);
        muhlisp_free_parser(&parser);
        exit(1);
    }

    for(;;) {
        char* input = readline("muhlisp> ");
        if(input == NULL) { // input is EOF
            break;
        }

        mpc_result_t result;
        if(muhlisp_parse_input(&parser, input, &result)) {
            double value;

            if(!eval_ast(result.output, &value)) {
                printf("%f\n", value);
            } else {
                puts("Error evaluating expression");
            }
            mpc_ast_delete(result.output);
        } else {
            mpc_err_print(result.error);
            mpc_err_delete(result.error);
        }

        add_history(input);
        free(input);
    }

    muhlisp_free_parser(&parser);
    puts("\nExiting muhlisp...");

    return 0;
}
