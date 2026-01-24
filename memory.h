#include <stdio.h>
#include <stdlib.h>

typedef struct var {
	char name;
	int start;
	int end;
} var;

typedef struct FreeBlock {
	int start;
	int size;
	struct FreeBlock* next; 
} FreeBlock;

typedef struct Command {
    char* op;
    char a;
    char b;
} Command;
