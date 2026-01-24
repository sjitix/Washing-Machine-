#include <stdio.h>
#include <stdlib.h>
#include "declarations.h"

void error_undefined_variable(void) {
    // after trying to access a variable that hasn't yet been created with MAL
    fprintf(stderr, "Tried to use a variable that does not exist.\n");
    exit(0);
}

void error_out_of_memory(void) {
    // after trying to allocate memory currently not available in the system
    fprintf(stderr, "Not enough memory.\n");
    exit(0);
}

void error_diff_length(void) {
    // after trying to apply AND/XOR to variables of different length
    fprintf(stderr, "Logic operation between sequences of different lengths.\n");
    exit(0);
}

void error_wrong_memory(void) {
    // after trying to access a cell which is not within the variable range
    fprintf(stderr, "Wrong memory access.\n");
    exit(0);
}