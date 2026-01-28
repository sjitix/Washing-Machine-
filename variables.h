#ifndef VARIABLES_H
#define VARIABLES_H

//were including memory.h to get the type memory MANAGER
#include "memory.h"




//Variables ADT
typedef struct internalVar * Variable;


void var_init (memoryManager memory);

Variable var_get(char name);

int var_allocate(char name, int size);

int var_free(char name);

int var_exists(char name);

int var_read_at(Variable v, int index);

void var_write_at(Variable v, int index, int value);

int var_size(Variable v);

#endif