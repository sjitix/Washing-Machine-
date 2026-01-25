
#include <stdio.h>
#include <stdlib.h>
#include "memory.h"


int Main_Array[100];

FreeBlock* freelist;

var var_table[256];


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

                                //if size of node is exact
                                if(current -> size == size)
                                {

                                        if(previous == NULL)

                                               { freelist = current -> next;
						free(current);
					       }
                                        else{
                                                previous -> next = current -> next;

                                                //remove the node?
                                                free(current);
                                        }

                                }
                                //size of node is bigger
                                else {
                                        //add to the start
                                        current -> start = current -> start + size;

                                        //substract to have remaining space in the node
                                        current -> size = current -> size - size;


                                        }

                                //initialize with 0
                                for (int i = start_free_node;  i < start_free_node + size; i++)
                                {
                                        Main_Array[i] = 0;
                                }


                                //update variables
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

        //check if freelist is null
        if (freelist == NULL)
        {

                //insert at head
                new_free_node -> next = NULL;

                freelist = new_free_node;

                coalesce(previous , new_free_node);

                return 1;


        }

        //go find empty node
        while(current != NULL)
        {
                if(new_free_node -> start < current -> start    && previous == NULL)
                {


                  new_free_node -> next = current ;

                  freelist = new_free_node;

                  coalesce(previous , new_free_node);

                  //start coaelesce

                  return 1;
                }

                else if( previous != NULL)
                {
                        if(previous -> start <  new_free_node -> start && new_free_node -> start < current -> start)
                         {
                                new_free_node -> next = current;

                                previous -> next = new_free_node;

                                //start coalesce
                                coalesce(previous, new_free_node);

                                return 1;
                         }

                }

                previous = current;

                current = current -> next;

        }

        //put it at the end of the empty list?

                previous -> next = new_free_node;

                new_free_node -> next = NULL;

                coalesce(previous, new_free_node);

                return 1;







}

int var_exists(char name)
{
        if ( var_table[name].start != -1) return 1;
        else return 0;
}

int var_read(char name, int index)
{
        return Main_Array[var_table[name].start + index];
}

void var_write(char name , int index, int value)
{

        Main_Array[var_table[name].start + index] = value;

}

int var_get_size(char name)
{
        return var_table[name].size;
}
