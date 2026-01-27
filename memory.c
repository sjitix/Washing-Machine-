#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

int Main_Array[100];

struct internalVar {
	int start;
	int size;
};

typedef struct FreeBlock {
	int start;
	int size;
	struct FreeBlock* next;
} FreeBlock;

// list of free spaces
FreeBlock* freelist;

struct internalVar var_table[256];

void memory_init(void)
{
	freelist = malloc ( sizeof(struct FreeBlock));
	//initially the whole list is one free block
	freelist -> start = 0;
	freelist -> size = 100;
	freelist -> next = NULL;

	//start of a variable = -1 means variable doesn't exist
	for(int i = 0 ; i < 256 ; i++)var_table[i].start = -1;
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

int var_allocate(char name , int size)
{
        FreeBlock * current;
        FreeBlock * previous;

        current = freelist;
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
                                        freelist = current -> next;
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
                        }

                        //initialize with 0 - the actual alocation
                        for (int i = start_free_node;  i < start_free_node + size; i++)
                        {
                                Main_Array[i] = 0;
                        }

                        //update variables
                        var_table[(int)name].start = start_free_node;
                        var_table[(int)name].size = size;

                        return 1;
                }

                previous = current;
                current = current -> next;
        }

        return 0;
}

int var_free(char name)
{
        FreeBlock * new_free_node;

        //if the variable already doesnt exist , cannot free it
        if(var_table[(int)name].start == -1) return 0;

        new_free_node = malloc (sizeof(struct FreeBlock));

        FreeBlock * current;
        FreeBlock * previous;

        current = freelist;
        previous = NULL;

        //set new free node in the list based on the freed variable
        new_free_node -> start  = var_table[(int)name].start;
        new_free_node -> size = var_table[(int)name].size;

        //variable doesnt exist
        var_table[(int)name].start = -1;

        // if freelist is empty already
        if (freelist == NULL)
        {
                //insert at head
                new_free_node -> next = NULL;
                freelist = new_free_node;
                coalesce(previous , new_free_node);
                return 1;
        }

        //find the right position to insert new free node
        while(current != NULL)
        {
                //if smaller than first empty space
                if(new_free_node -> start < current -> start && previous == NULL)
                {
                        //add it as the first node of freelist
                        new_free_node -> next = current ;
                        freelist = new_free_node;
                        //merge freespaces
                        coalesce(previous , new_free_node);
                        return 1;
                }

                else if( previous != NULL)
                {
                        //if it new free space bewteen previous and current
                        // add between them
                        coalesce(previous, new_free_node);
                        return 1;
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

        return 1;
}

int var_exists(char name)
{
        if ( var_table[(int)name].start != -1) return 1;
        else return 0;
}

Variable var_get(char name)
{
        if (var_table[(int)name].start == -1) return NULL;
        return &var_table[(int)name];
}

int var_read_at(Variable v, int index)
{
        return Main_Array[v->start + index];
}

void var_write_at(Variable v, int index, int value)
{
        Main_Array[v->start + index] = value;
}

int var_size(Variable v)
{
        return v->size;
}

void free_list(void) {
	FreeBlock* current = freelist;
	while (freelist != NULL) {
		current = freelist;
		freelist = freelist->next;
		free(current);
	}
}
