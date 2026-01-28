#include <stdio.h>
#include "memory.h"
#include "variables.h"
#include "parser.h"
#include "executor.h"



int main(int argc, char ** argv) {

    memoryManager memory = memory_create();
    
    var_init(memory);

    int count = parse(argv[1]);

    for (int i = 0; i < count; i++) {
        execute(i);
    }

    free_all_lists(memory);	
    
    return 0;
}

