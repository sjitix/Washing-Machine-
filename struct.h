// this file might need to be reorganized into another file or renamed or something, it's just for the struct Command data strcuture construction
#ifndef STRUCT_H
#define STRUCT_H


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


typedef struct Command {
    int op;
    char var1;
    char var2;

    int number;
} Command;

#endif
