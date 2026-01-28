
#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "errors.h"
#include "variables.h"

struct internalVar {
	int start;
	int size;
        char name;
};

struct internalVar var_table[256];

void var_init()
{
    //start of a variable = -1 means variable doesn't exist
	for(int i = 0 ; i < 256 ; i++)var_table[i].start = -1;
}




int var_allocate(char name , int size)
{

        if ( size <= 0) error_wrong_memory();

        int start = mem_allocate(size);

        if(start == -1) error_wrong_memory(); 

        var_table[(int)name].start = start;
        var_table[(int)name].size = size;
        var_table[(int)name].name = name;

        return 1;
}

int var_free(char name)
{
       

        //if the variable already doesnt exist , cannot free it
        if(var_table[(int)name].start == -1) error_undefined_variable();

        mem_free(var_table[(int)name].start, var_table[(int)name].size);

        //variable doesnt exist
        var_table[(int)name].start = -1;

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
        if (v == NULL || !var_exists(v->name)) error_undefined_variable();
        return Main_Array[v->start + index];
}

void var_write_at(Variable v, int index, int value)
{
        if (v == NULL || !var_exists(v->name)) error_undefined_variable();
        Main_Array[v->start + index] = value;
}

int var_size(Variable v)
{
        return v->size;
}