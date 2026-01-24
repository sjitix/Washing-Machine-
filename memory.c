#include <stdio.h>
#include <stdlib.h>
#include "memory.h"


FreeBlock* freelist;

void memory_init()
{    
	int Main_Array[100];

	for (int i = 0; i < 100; i++) Main_Array[i] = 0;

}

bool var_allocate(char name , int size)
{
	int name[size];
	int start;
	start = freelist.start;

	Main_Array[start] = nam

	


}


