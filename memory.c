#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "errors.h"

// list of free spaces

typedef struct FreeBlock {
	int start;
	int size;
	struct FreeBlock* next;
} FreeBlock;




typedef struct MemoryUnit{


        int start;
        int size;
        int * array;
        struct MemoryUnit* next;


}MemoryUnits;



typedef struct MemoryManager{
       
        int Main_Array[100];

        FreeBlock* freelist;

       MemoryUnits * units_in_use;


}MemoryManager;


//this function merges adjacent blocks of free space in the freelsit
void coalesce (FreeBlock *  previous , FreeBlock * new_node)
{
        FreeBlock * temp;

        //if the end of the node is equal to the start of the next
        //merge them
        if( (new_node -> next != NULL) && (new_node -> start + new_node -> size == new_node -> next -> start))
        {
                temp = new_node -> next;

                new_node -> size = new_node -> size + new_node -> next -> size;

                new_node -> next = new_node -> next -> next;

                free(temp);
        }

        //if the start of the node is equal to the end of the previous
        //merge them
        if( (previous != NULL) && (previous -> start + previous -> size == new_node -> start) )
        {
                previous -> size = previous -> size + new_node -> size;

                previous -> next = new_node -> next;

                free(new_node);
        }
}


memoryManager memory_create(void)
{
        memoryManager memory = malloc(sizeof(struct MemoryManager));
        memory -> freelist = malloc ( sizeof(struct FreeBlock));
	//initially the whole list is one free block
	memory -> freelist -> start = 0;
	memory -> freelist -> size = 100;
	memory -> freelist -> next = NULL;

        memory -> units_in_use = NULL;

        return memory;

	
}

memoryUnit mem_allocate(memoryManager memory , int size)
{
        
 

        FreeBlock * current;
        FreeBlock * previous;

        current = memory -> freelist;
        previous = NULL;

        int start_free_node;

        //iterate trough the free list
        while(current != NULL)
        {
                //if the size of the node is >= than the size needed
                //suitable empty space has been found
                if (current -> size >= size)
                {
                        start_free_node = current -> start;

                        //if size of node is exact
                        if(current -> size == size)
                        {
                                //remove node from freelist
                                //case1: its the first free space in the list
                                if(previous == NULL)
                                {
                                       memory -> freelist = current -> next;
                                       free(current);
                                }
                                //case2: its not first node in freelist
                                else
                                {
                                        previous -> next = current -> next;
                                        free(current);
                                }
                        }
                        //size of node is bigger -> truncate the node
                        else
                        {
                                //add to the start
                                current -> start = current -> start + size;
                                //substract to have remaining space in the node
                                current -> size = current -> size - size;

                               //free(current);
                        }

                        //initialize with 0 - the actual alocation
                        for (int i = start_free_node;  i < start_free_node + size; i++)
                        {
                                memory -> Main_Array[i] = 0;
                        }

                        //update variables
                       

                        //create the unit
                        memoryUnit unit = malloc(sizeof(struct MemoryUnit));
                        
                        unit -> start = start_free_node;
                        unit -> size = size;
                        unit -> array = memory -> Main_Array;

                        //add to units list , at the start
                        unit -> next = memory -> units_in_use;

                        memory -> units_in_use = unit;

                        return unit;


                }

                previous = current;
                current = current -> next;
        }


        //unit wasnt allocated therefore it doesnt exist , returns NULL
        return NULL;
}

void mem_free(memoryManager memory , memoryUnit unit)
{


        //get start and size from unit
        int start = unit -> start;
        int size = unit -> size;


 //remove unit from units_in_ use
       
        MemoryUnits * current_unit = memory -> units_in_use;
        MemoryUnits * previous_unit = NULL;

        //iterate till you find the unit
        while(current_unit != NULL && current_unit != unit)
        {
                previous_unit = current_unit;
                current_unit = current_unit -> next;
        }

        //if the unit has been found remove it
        if (current_unit != NULL)
        {
                if (previous_unit == NULL)
                {
                        memory -> units_in_use = unit -> next;
                }
                else {
                        previous_unit -> next = unit -> next;
                }



        }

        free(unit);

       
       
 //adding new node to free list

        FreeBlock * new_free_node;

        new_free_node = malloc (sizeof(struct FreeBlock));

        FreeBlock * current;
        FreeBlock * previous;

        current = memory -> freelist;
        previous = NULL;

        //set new free node in the list based on the freed variable
        new_free_node -> start  = start;
        new_free_node -> size = size;

  

        // if freelist is empty already
        if (memory -> freelist == NULL)
        {
                //insert at head
                new_free_node -> next = NULL;
                memory -> freelist = new_free_node;

                return ;
        }

        //find the right position to insert new free node
        while(current != NULL)
        {
                //if smaller than first empty space
                if(new_free_node -> start < current -> start && previous == NULL)
                {
                        //add it as the first node of freelist
                        new_free_node -> next = current ;
                        memory -> freelist = new_free_node;
                        //merge freespaces
                        coalesce(previous , new_free_node);
                        return ;
                }

                else if( previous != NULL && new_free_node -> start < current -> start)
                {
                        //if it new free space bewteen previous and current
                        // add between them
                        previous -> next = new_free_node;
                        new_free_node -> next = current;
                        coalesce(previous, new_free_node);
                        return ;
                }

                previous = current;
                current = current -> next;
        }

      //if no suitable space has been found
        //put it at the end of the empty list

        previous -> next = new_free_node;

        new_free_node -> next = NULL;

        //merge freespaces
        coalesce(previous, new_free_node);


        return ;
}





int mem_read (memoryUnit unit, int index)
{
        return unit -> array[unit -> start + index];
}

void mem_write(memoryUnit unit , int index , int value)
{
        unit -> array [unit -> start + index ] = value;
}

int mem_size(memoryUnit unit )
{
        return unit -> size;
}


void free_all_lists(memoryManager memory ) {

        // free all units in use
        MemoryUnits* unit = memory -> units_in_use;
        while (unit != NULL) {
         MemoryUnits* temp = unit;
         unit = unit -> next;
         free(temp);
        }

        //free the freelist
	FreeBlock* current = memory -> freelist;
	while (memory ->freelist != NULL) {
		current = memory -> freelist;
		memory -> freelist = memory -> freelist->next;
		free(current);
	}

        //free the memory manager itself
        free(memory);
}


//shifts free memory , when the free spaces are not adjacent
void mem_compact (memoryManager memory)
{
        int new_position = 0;

        MemoryUnits * unit = memory -> units_in_use;

        //move each unit s data  to new position
        while(unit != NULL)
        {
                //only move if not already in the right place

                if(unit -> start != new_position)
                {
                        //copy data to new position
                        for(int i = 0; i < unit -> size ; i++)
                        {
                                memory -> Main_Array[new_position + i] = memory -> Main_Array[unit -> start + i];
                        }

                        //update unit start to new position

                        unit -> start = new_position;
                }
                //next position is after this unit
                new_position = new_position + unit -> size;

                unit = unit -> next;
        }


            // free old freelist
    FreeBlock* current = memory->freelist;
    while (current != NULL)
    {
        FreeBlock* temp = current;
        current = current->next;
        free(temp);
    }

    // create single free block for remaining space
    if (new_position < 100)
    {
        memory->freelist = malloc(sizeof(FreeBlock));
        memory->freelist->start = new_position;
        memory->freelist->size = 100 - new_position;
        memory->freelist->next = NULL;
    }
    else
    {
        // array is completely full
        memory->freelist = NULL;
    }
}



