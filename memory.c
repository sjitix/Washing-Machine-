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


typedef struct MemoryManager{
       
        int Main_Array[100];

        FreeBlock* freelist;

}MemoryManager;








memoryManager memory_create(void)
{
        memoryManager memory = malloc(sizeof(struct MemoryManager));
        memory -> freelist = malloc ( sizeof(struct FreeBlock));
	//initially the whole list is one free block
	memory -> freelist -> start = 0;
	memory -> freelist -> size = 100;
	memory -> freelist -> next = NULL;

        return memory;

	
}

int mem_allocate(memoryManager memory , int size)
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
                       



                        return start_free_node;
                }

                previous = current;
                current = current -> next;
        }


        
        return -1;
}

void mem_free(memoryManager memory , int start , int size)
{
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

                else if( previous != NULL)
                {
                        //if it new free space bewteen previous and current
                        // add between them
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



//this function merges adjacent blocks of free space
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



void free_list(memoryManager memory ) {
	FreeBlock* current = memory -> freelist;
	while (memory ->freelist != NULL) {
		current = memory -> freelist;
		memory -> freelist = memory -> freelist->next;
		free(current);
	}
}
