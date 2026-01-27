#ifndef MEMORY_H
#define MEMORY_H

typedef struct internalVar * Variable;

void memory_init(void);

Variable var_get(char name);

int var_allocate(char name, int size);

int var_free(char name);

int var_exists(char name);

int var_read_at(Variable v, int index);

void var_write_at(Variable v, int index, int value);

int var_size(Variable v);

/* EFFECT: Frees the memory*/ 
void free_list(void);

#endif
