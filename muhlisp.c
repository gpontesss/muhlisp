#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>
#include <string.h>

#include "mpc.h"
#include "parsing.h"

#include "eval.h"


int main(int argc, char** argv) {
    muhlisp_parser_t parser;
    mpc_err_t* err = muhlisp_init_parser(&parser);

    if(err != NULL) {
        mpc_err_print(err);
        muhlisp_free_parser(&parser);
        exit(1);
    }

    puts("muhlisp version 0.0.1");
    puts("press ctrl+c to quit");

    for(;;) {
        char* input = readline("muhlisp> ");
        if(input == NULL) { // input is EOF
            break;
        }

        mpc_result_t result;
        if(muhlisp_parse_input(&parser, input, &result)) {
            muhlisp_val_t val;
            eval_ast(result.output, &val);

            char* str = muhlisp_val_str(&val);
            if(str != NULL) {
                puts(str);
            } else {
                puts("Can't evaluate value string.");
            }

            // muhlisp_val_free(&val);
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
