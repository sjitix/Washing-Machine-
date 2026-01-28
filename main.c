#include <stdio.h>
#include "memory.h"

#include "parser.h"
#include "executor.h"



int main(int argc, char ** argv) {


    memory_init();

    for (int i = 0; i < count; i++) {
        execute(i);
    }

    free_list();	
    
    return 0;
}

