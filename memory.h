#ifndef MEMORY_H
#define MEMORY_H

// memory manager ADT - a memory system that manages 100 integer cells
//contains the array, freelist, and list of allocated units
typedef struct MemoryManager * memoryManager;


// memory unit ADT - represents one allocated block of memory
// stores position and size inside each unit 
typedef struct MemoryUnit * memoryUnit;

// creates and initializes a new memory manager
memoryManager memory_create(void);

// allocates a block of the given size returns NULL if not enough space
memoryUnit mem_allocate(memoryManager memory, int size);

// frees a memory unit and returns space to the freelist
void mem_free(memoryManager memory, memoryUnit unit);

// moves all allocated blocks together, removing gaps
void mem_compact(memoryManager memory);

// frees all memory used by the manager
void free_all_lists(memoryManager memory);

// reads value at index inside the unit
int mem_read(memoryUnit unit, int index);

// writes value at index within the unit
void mem_write(memoryUnit unit, int index, int value);

// returns the size of the unit
int mem_size(memoryUnit unit);

#endif
