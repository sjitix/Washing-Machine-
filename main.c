#include <stdio.h>
#include "memory.h"

#include "parser.h"
#include "executor.h"



int main(int argc, char ** argv) {


    memory_init();

    int count = parse(argv[1]);

    for (int i = 0; i < count; i++) {
        execute(i);
    }

    return 0;
}

