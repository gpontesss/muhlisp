#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>

int main(int argc, char** argv) {

    puts("muhlisp version 0.0.1");
    puts("press ctrl+c to quit");

    for(;;) {
        char* input = readline("muhlisp> ");
        add_history(input);
        printf("%s\n", input);
        free(input);
    }

    return 0;
}
