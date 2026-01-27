#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "memory.h"
#include "errors.h"


void execute(int index) {
	int i;
	int result;

	switch(get_cmd_op(index)) {
		case ASS:
			//assigns n to the first space of x
			var_write(get_cmd_var1(index), 0, get_cmd_number(index));
			break;

		case INC:
			if(!var_exists(get_cmd_var1(index)))
					error_undefined_variable();
			if(get_cmd_number(index) < 0 || get_cmd_number(index) >= var_get_size(get_cmd_var1(index)))
					error_wrong_memory();
			//writes in the nth space denoted by x,the same number add 1
			var_write(get_cmd_var1(index), get_cmd_number(index) , var_read(get_cmd_var1(index), get_cmd_number(index) ) + 1);
			break;

		case DEC:
			//writes in the nth space denoted by x, the same number minus 1
			var_write(get_cmd_var1(index), get_cmd_number(index) , var_read(get_cmd_var1(index), get_cmd_number(index) ) - 1);
			break;

		case MAL:
			//	use memory_allocate!! wait for implementation
			 result = var_allocate(get_cmd_var1(index), get_cmd_number(index));
			if (result == 0)
				error_out_of_memory();
			break;

		case PRI:
			//reads the nth space denoted by x and prints it
			printf("%d\n", var_read(get_cmd_var1(index), get_cmd_number(index) ));
			break;

		case ADD:
			//overwrites the first cell of x with the result of the addition of the first cells of x and y
			var_write(get_cmd_var1(index), 0, var_read(get_cmd_var1(index), 0) + var_read(get_cmd_var2(index), 0));
			break;

		case SUB:
			//overwrites the first cell of x with the result of the subtraction of the first cells of x and y
			var_write(get_cmd_var1(index), 0, var_read(get_cmd_var1(index), 0) - var_read(get_cmd_var2(index), 0));
			break;

		case MUL:
			//overwrites the first cell of x with the result of the multiplication of the first cells of x and y
			var_write(get_cmd_var1(index), 0, var_read(get_cmd_var1(index), 0) * var_read(get_cmd_var2(index), 0));
			break;

		case AND:
			//overwriting each element of x[i] with the value of (x[i]*y[i]%2)
			for (i = 0; i < var_get_size(get_cmd_var1(index)); i++) {
				var_write(get_cmd_var1(index), i, (var_read(get_cmd_var1(index), i) * var_read(get_cmd_var2(index), i)) % 2);
			}
			break;

		case XOR:
			//overwriting each element of x[i] with the value of (x[i]+y[i]%2)
			for (i = 0; i < var_get_size(get_cmd_var1(index)); i++) {
				var_write(get_cmd_var1(index), i, (var_read(get_cmd_var1(index), i) + var_read(get_cmd_var2(index), i)) % 2);
			}
			break;

		case FRE:
			//	use memory_free()!! wait for implementation
			var_free(get_cmd_var1(index));
			break;

		case PRA:
			//prints contents of a variable
			printf("[");
			for (i = 0; i < var_get_size(get_cmd_var1(index)); i++) {
				printf(" %d", var_read(get_cmd_var1(index), i));
			}
			printf(" ]\n");
			break;
	}
}
