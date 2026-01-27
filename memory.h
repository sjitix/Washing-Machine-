#ifndef MEMORY_H
#define MEMORY_H




void memory_init(void);



int var_allocate(char name, int size);

int var_free(char name);

int var_exists(char name);

int var_read(char name, int index);

void var_write(char name, int index, int value);

int var_get_size(char name);

#endif
