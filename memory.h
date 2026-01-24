#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

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
