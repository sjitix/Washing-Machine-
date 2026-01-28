
#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "errors.h"
#include "variables.h"

//not global only private to this file 
//this memory type only points to the memory manager itself
static memoryManager mem;

struct internalVar {
	
        char name;
        memoryUnit unit;
};

struct internalVar var_table[256];

void var_init(memoryManager memory )
{
        //memory is passed from main,c
        //we save it in mem so other functions in 
        // variables.c can use it later

        mem = memory;

    //start of a variable = -1 means variable doesn't exist
	for(int i = 0 ; i < 256 ; i++)var_table[i].unit = NULL;
}




int var_allocate(char name , int size)
{

        if ( size <= 0) error_wrong_memory();

        memoryUnit unit  = mem_allocate(mem , size);

        if(unit == NULL) error_out_of_memory();

        var_table[(int)name].unit = unit;
 
        var_table[(int)name].name = name;

        return 1;
}

int var_free(char name)
{
       

        //if the variable already doesnt exist , cannot free it
        if(var_table[(int)name].unit == NULL) error_undefined_variable();

        mem_free(mem , var_table[(int)name].unit);

        //variable doesnt exist
        var_table[(int)name].unit = NULL;

        return 1;

      
}

int var_exists(char name)
{
        if ( var_table[(int)name].unit != NULL) return 1;
        else return 0;
}

Variable var_get(char name)
{
        if (var_table[(int)name].unit == NULL) return NULL;
        return &var_table[(int)name];
}

int var_read_at(Variable v, int index)
{
        if (v == NULL || v -> unit == NULL) error_undefined_variable();
        return mem_read(v -> unit , index);
}

void var_write_at(Variable v, int index, int value)
{
        if (v == NULL || v -> unit == NULL) error_undefined_variable();
        mem_write (v -> unit , index, value);
}

int var_size(Variable v)
{
        return mem_size (v -> unit);
}