#ifndef PARSER_H
#define PARSER_H

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



typedef struct internalCommand * Command;

int parse(const char * filename);

Command get_command(int index);

int cmd_get_op(Command cmd);

char cmd_get_var1(Command cmd);

char cmd_get_var2(Command cmd);

int cmd_get_number(Command cmd);

#endif
