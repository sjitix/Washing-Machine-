#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "memory.h"
#include "errors.h"

void execute(int index) {
	int i;
	int result;

	Command cmd = get_command(index);

	Variable v1 = var_get(cmd_get_var1(cmd));
	Variable v2 = var_get(cmd_get_var2(cmd));

	switch(cmd_get_op(cmd)) {
		case ASS:
			//assigns n to the first space of x
			var_write_at(v1, 0, cmd_get_number(cmd));
			break;

		case INC:
			if(v1 == NULL)
					error_undefined_variable();
			if(cmd_get_number(cmd) < 0 || cmd_get_number(cmd) >= var_size(v1))
					error_wrong_memory();
			//writes in the nth space denoted by x,the same number add 1
			var_write_at(v1, cmd_get_number(cmd), var_read_at(v1, cmd_get_number(cmd)) + 1);
			break;

		case DEC:
			//writes in the nth space denoted by x, the same number minus 1
			var_write_at(v1, cmd_get_number(cmd), var_read_at(v1, cmd_get_number(cmd)) - 1);
			break;

		case MAL:
			result = var_allocate(cmd_get_var1(cmd), cmd_get_number(cmd));
			if (result == 0)
				error_out_of_memory();
			break;

		case PRI:
			//reads the nth space denoted by x and prints it
			printf("%d\n", var_read_at(v1, cmd_get_number(cmd)));
			break;

		case ADD:
			//overwrites the first cell of x with the result of the addition of the first cells of x and y
			var_write_at(v1, 0, var_read_at(v1, 0) + var_read_at(v2, 0));
			break;

		case SUB:
			//overwrites the first cell of x with the result of the subtraction of the first cells of x and y
			var_write_at(v1, 0, var_read_at(v1, 0) - var_read_at(v2, 0));
			break;

		case MUL:
			//overwrites the first cell of x with the result of the multiplication of the first cells of x and y
			var_write_at(v1, 0, var_read_at(v1, 0) * var_read_at(v2, 0));
			break;

		case AND:
			//overwriting each element of x[i] with the value of (x[i]*y[i]%2)
			for (i = 0; i < var_size(v1); i++) {
				var_write_at(v1, i, (var_read_at(v1, i) * var_read_at(v2, i)) % 2);
			}
			break;

		case XOR:
			//overwriting each element of x[i] with the value of (x[i]+y[i]%2)
			for (i = 0; i < var_size(v1); i++) {
				var_write_at(v1, i, (var_read_at(v1, i) + var_read_at(v2, i)) % 2);
			}
			break;

		case FRE:
			var_free(cmd_get_var1(cmd));
			break;

		case PRA:
			//prints contents of a variable
			printf("[");
			for (i = 0; i < var_size(v1); i++) {
				printf(" %d", var_read_at(v1, i));
			}
			printf(" ]\n");
			break;
	}
}
