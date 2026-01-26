#include <stdio.h>
#include <stdlib.h>

typedef struct var {
	char name;
	int start;
	int end;
} var;

// FOLLOWING NEEDS ANA'S APPROVAL
// declares main memory 
int memory[100];
for (int i=0; i<100; i++)
	memory[i] = 0;
// declares array of pointers to variables // SHOULD THIS BE A LINKED LIST?
var* varArr // we don't know how many will be called so probably yeah

// probably going to change this to just be an int next_free_space or something
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