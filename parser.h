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



int parse(const char * filename );

//Getters for executor to use

int get_cmd_op(int index);

char get_cmd_var1(int index);

char get_cmd_var2(int index);

int get_cmd_number(int index);



#endif
