#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

typedef struct var {
	int start;
	int size;
} var;

typedef struct FreeBlock {
	int start;
	int size;
	struct FreeBlock* next;
} FreeBlock;

void memory_init(void);

void coalesce (FreeBlock * previous , FreeBlock * new_node);

int var_allocate(char name, int size);

int var_free(char name);

int var_exists(char name);

int var_read(char name, int index);

void var_write(char name, int index, int value);

int var_get_size(char name);

#endif
