#ifndef MEMORY_H
#define MEMORY_H



//Memory Manager ADT
typedef struct  MemoryManager *  memoryManager;

//Memory Unit ADT

typedef struct MemoryUnit * memoryUnit; 


memoryManager memory_create(void);

memoryUnit mem_allocate(memoryManager memory , int size);

void mem_free(memoryManager memory ,memoryUnit unit);

void mem_compact(memoryManager memory);


//acess memory trough units

int mem_read(memoryUnit unit , int index);

void mem_write(memoryUnit unit , int index , int value);

int mem_size(memoryUnit unit);





void free_all_lists(memoryManager memory);

#endif
