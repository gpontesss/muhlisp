#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>

#include "mpc.h"
#include "parsing.h"


int main(int argc, char** argv) {

    puts("muhlisp version 0.0.1");
    puts("press ctrl+c to quit");

    muhlisp_parser_t parser = muhlisp_create_parser();

    for(;;) {
        char* input = readline("muhlisp> ");
        if(input == NULL) { // input is EOF
            break;
        }

        mpc_result_t result;
        if(muhlisp_parse_input(&parser, input, &result)) {
            mpc_ast_print(result.output);
            mpc_ast_delete(result.output);
        } else {
            mpc_err_print(result.error);
            mpc_err_delete(result.error);
        }

        add_history(input);
        free(input);
    }

    muhlisp_free_parser(parser);
    puts("\nExiting muhlisp...");

    return 0;
}
