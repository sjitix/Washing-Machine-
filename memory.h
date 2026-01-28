#ifndef MEMORY_H
#define MEMORY_H



//Memory Manager ADT
typedef struct  MemoryManager *  memoryManager;


memoryManager memory_create(void);

int mem_allocate(memoryManager memory , int size);

void mem_free(memoryManager memory ,int start , int size);





void free_list(memoryManager memory);

#endif
