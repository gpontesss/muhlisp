#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>
#include <string.h>

#include "./mpc.h"
#include "./parsing.h"


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

        muhlisp_val_t val;
        muhlisp_parser_read_val(&parser, input, &val);

        char* str = muhlisp_val_str(&val);
        if(str != NULL) {
            puts(str);
            free(str);
        } else {
            puts("Can't evaluate value string.");
        }
        muhlisp_val_free(&val);

        add_history(input);
        free(input);
    }

    muhlisp_free_parser(&parser);
    puts("\nExiting muhlisp...");

    return 0;
}
