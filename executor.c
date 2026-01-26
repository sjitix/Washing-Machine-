#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "memory.h"

void execute(Command cmd) {
	int i;

	switch(cmd.op) {
		case ASS:
			//assigns n to the first space of x
			var_write(cmd.var1, 0, cmd.number);
			break;

		case INC:
			//writes in the nth space denoted by x,the same number add 1
			var_write(cmd.var1, cmd.number - 1, var_read(cmd.var1, cmd.number - 1) + 1);
			break;

		case DEC:
			//writes in the nth space denoted by x, the same number minus 1
			var_write(cmd.var1, cmd.number - 1, var_read(cmd.var1, cmd.number - 1) - 1);
			break;

		case MAL:
			//	use memory_allocate!! wait for implementation
			var_allocate(cmd.var1, cmd.number);
			break;

		case PRI:
			//reads the nth space denoted by x and prints it
			printf("%d\n", var_read(cmd.var1, cmd.number - 1));
			break;

		case ADD:
			//overwrites the first cell of x with the result of the addition of the first cells of x and y
			var_write(cmd.var1, 0, var_read(cmd.var1, 0) + var_read(cmd.var2, 0));
			break;

		case SUB:
			//overwrites the first cell of x with the result of the subtraction of the first cells of x and y
			var_write(cmd.var1, 0, var_read(cmd.var1, 0) - var_read(cmd.var2, 0));
			break;

		case MUL:
			//overwrites the first cell of x with the result of the multiplication of the first cells of x and y
			var_write(cmd.var1, 0, var_read(cmd.var1, 0) * var_read(cmd.var2, 0));
			break;

		case AND:
			//overwriting each element of x[i] with the value of (x[i]*y[i]%2)
			for (i = 0; i < var_get_size(cmd.var1); i++) {
				var_write(cmd.var1, i, (var_read(cmd.var1, i) * var_read(cmd.var2, i)) % 2);
			}
			break;

		case XOR:
			//overwriting each element of x[i] with the value of (x[i]+y[i]%2)
			for (i = 0; i < var_get_size(cmd.var1); i++) {
				var_write(cmd.var1, i, (var_read(cmd.var1, i) + var_read(cmd.var2, i)) % 2);
			}
			break;

		case FRE:
			//	use memory_free()!! wait for implementation
			var_free(cmd.var1);
			break;

		case PRA:
			//prints contents of a variable
			printf("[");
			for (i = 0; i < var_get_size(cmd.var1); i++) {
				printf(" %d", var_read(cmd.var1, i));
			}
			printf(" ]\n");
			break;
	}
}
