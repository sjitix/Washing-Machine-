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

FreeBlock* freelist;

struct internalVar var_table[256];

void memory_init()
{
	freelist = malloc ( sizeof(struct FreeBlock));
	freelist -> start = 0;
	freelist -> size = 100;
	freelist -> next = NULL;

	for(int i = 0 ; i < 256 ; i++)var_table[i].start = -1;
}

void coalesce (FreeBlock *  previous , FreeBlock * new_node)
{
        FreeBlock * temp;

        if( (new_node -> next != NULL) && (new_node -> start + new_node -> size == new_node -> next -> start))
        {
                temp = new_node -> next;

                new_node -> size = new_node -> size + new_node -> next -> size;

                new_node -> next = new_node -> next -> next;

                free(temp);
        }

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

        while(current != NULL)
        {
                if (current -> size >= size)
                {
                        start_free_node = current -> start;

                        if(current -> size == size)
                        {
                                if(previous == NULL)
                                {
                                        freelist = current -> next;
                                        free(current);
                                }
                                else
                                {
                                        previous -> next = current -> next;
                                        free(current);
                                }
                        }
                        else
                        {
                                current -> start = current -> start + size;
                                current -> size = current -> size - size;
                        }

                        for (int i = start_free_node;  i < start_free_node + size; i++)
                        {
                                Main_Array[i] = 0;
                        }

                        var_table[name].start = start_free_node;
                        var_table[name].size = size;

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

        if(var_table[name].start == -1) return 0;

        new_free_node = malloc (sizeof(struct FreeBlock));

        FreeBlock * current;
        FreeBlock * previous;

        current = freelist;
        previous = NULL;

        new_free_node -> start  = var_table[name].start;
        new_free_node -> size = var_table[name].size;

        var_table[name].start = -1;

        if (freelist == NULL)
        {
                new_free_node -> next = NULL;
                freelist = new_free_node;
                coalesce(previous , new_free_node);
                return 1;
        }

        while(current != NULL)
        {
                if(new_free_node -> start < current -> start && previous == NULL)
                {
                        new_free_node -> next = current ;
                        freelist = new_free_node;
                        coalesce(previous , new_free_node);
                        return 1;
                }

                else if( previous != NULL)
                {
                        coalesce(previous, new_free_node);
                        return 1;
                }

                previous = current;
                current = current -> next;
        }

        previous -> next = new_free_node;
        new_free_node -> next = NULL;
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
