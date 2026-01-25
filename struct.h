// this file might need to be reorganized into another file or renamed or something, it's just for the struct Command data strcuture construction
#ifndef STRUCT_H
#define STRUCT_H


#define OP_MAL 0
#define OP_ASS 1
#define OP_INC 2
#define OP_DEC 3
#define OP_PRI 4
#define OP_ADD 5
#define OP_SUB 6
#define OP_MUL 7
#define OP_AND 8
#define OP_XOR 9
#define OP_FRE 10
#define OP_PRA 11



typedef struct Command {
    int op;
    char var1;
    char var2;

    int number;
} Command;

#endif
