
all:
	gcc -Wall -pedantic -g main.c parser.c memory.c executor.c errors.c variables.c -o interpreter


