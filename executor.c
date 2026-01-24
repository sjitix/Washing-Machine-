#include <stdio.h>
#include <stdlib.h>
#include "declarations.h"

void execute(Command cmd) {

	switch(cmd.op) {
//VERY WIP!!! switch/command notation isei very wrong but actual function workings should be correct assuming the functions that are called also work. -gui
		case ASS:
			//assigns n to the first space of x
			memory_write(var_get_start(cmd.a),cmd.b);
			break;
		case INC:
			//writes in the nth space denoted by x,the same number add 1
			memory_write(var_get_start(cmd.a)+cmd.b-1,memory_read(var_get_start(cmd.a)+cmd.b-1)+1);
            break;
		case DEC:
			//writes in the nth space denoted by x, the same number minus 1
			memory_write(var_get_start(cmd.a)+cmd.b-1,memory_read(var_get_start(cmd.a)+cmd.b-1)-1);
            break;
		case MAL:
			//	use memory_allocate!! wait for implementation

            break;

		case PRI:
			//reads the nth space denoted by x and prints it
			printf("%d",memory_read(var_get_start(cmd.a)+cmd.b-1));
            break;

		case ADD:
			//overwrites the first cell of x with the result of the addition of the first cells of x and y
			memory_write(var_get_start(cmd.a),memory_read(var_get_start(cmd.a))+memory_read(var_get_start(cmd.b)));
            break;

		case SUB:
			//overwrites the first cell of x with the result of the subtraction of the first cells of x and y
			memory_write(var_get_start(cmd.a),memory_read(var_get_start(cmd.a))-memory_read(var_get_start(cmd.b)));
            break;

        case MUL:
			//overwrites the first cell of x with the result of the multiplication of the first cells of x and y
			memory_write(var_get_start(cmd.a),memory_read(var_get_start(cmd.a))*memory_read(var_get_start(cmd.b)));
            break;

		case AND:
			//overwriting each element of x[i] with the value of (x[i]*y[i]%2) 
			int i;
			for(i=1,i=var_get_size(cmd.a),i++){
				memory_write(var_get_start(cmd.a)+i-1,(memory_read(var_get_start(x)+i-1)*memory_read(var_get_start(cmd.b)+i-1))%2);
			}
            break;

		case XOR:
			//overwriting each element of x[i] with the value of (x[i]+y[i]%2)
                        int i;
                        for(i=1,i=var_get_size(cmd.a),i++){
                                memory_write(var_get_start(cmd.a)+i-1,(memory_read(var_get_start(cmd.a)+i-1)+memory_read(var_get_start(cmd.b)+i-1))%2);
                        }
            break;
        
		case FRE:
			//	use memory_free()!! wait for implementation
            
            break;

		case PRA:
			//prints contents of a variable
			int i;
			printf("[")
			for(i=1,i=var_get_size(cmd.a),i++){
				printf("%d ",memory_read(var_get_start(cmd.a)+i-1));
			}
			printf("]");
            break;

	}
}
