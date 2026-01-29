#ifndef PARSER_H
#define PARSER_H

// each command is stored as an integer
#define MAL 0
#define ASS 1
#define INC 2
#define DEC 3
#define PRI 4
#define ADD 5
#define SUB 6
#define MUL 7
#define AND 8
#define XOR 9
#define FRE 10
#define PRA 11

// represents a line of code in the mini-language
typedef struct internalCommand * Command;

// inputs the code file; outputs an integer representing the number of lines of code
// parses code into an array of commands in the sequence they are called in the mini-language
int parse(const char * filename);

// inputs the index of a given command in the array of commands; outputs that given command
Command get_command(int index);

// inputs command instance; outputs the operation (e.g. MAL, DEC, etc) carried out by that command
int cmd_get_op(Command cmd);

// inputs command instance; outputs the first parameter of the command
char cmd_get_var1(Command cmd);

// inputs command instance; outputs the second parameter of the command (if it is a character)
char cmd_get_var2(Command cmd);

// inputs command instance; outputs the second parameter of the command (if it is an integer)
int cmd_get_number(Command cmd);

#endif
