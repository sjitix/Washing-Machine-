#ifndef VARIABLES_H
#define VARIABLES_H

#include "memory.h"

// variable ADT  stores a variable name  and its memory unit
// to use it , first call var_allocate then use var_get to retrieve it for read/write operations
typedef struct internalVar * Variable;

// initializes the variable table and stores reference to the memory manager
void var_init(memoryManager memory);

// returns the variable with the given name, or NULL if it does not exist
Variable var_get(char name);

// allocates memory for a variable with the intput data ,name and size
int var_allocate(char name, int size);

// frees the memory allocated for that variable
int var_free(char name);

// returns 1 if the variable exists 0 if it doesnt
int var_exists(char name);

// reads value at index within the variable's memory
int var_read_at(Variable v, int index);

// writes value at index within the variable's memory
void var_write_at(Variable v, int index, int value);

// returns the size of the variable
int var_size(Variable v);

#endif
