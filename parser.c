#include <stdio.h>
#include <string.h>
#include "parser.h"

typedef struct internalCommand {
    int op;
    char var1;
    char var2;
    int number;
} command;



Command get_command(int index)
{
    return &command;
}

int cmd_get_op(Command cmd)
{
    return cmd->op;
}

char cmd_get_var1(Command cmd)
{
    return cmd->var1;
}

char cmd_get_var2(Command cmd)
{
    return cmd->var2;
}

int cmd_get_number(Command cmd)
{
    return cmd->number;
}

int parse (const char * filename)
{
    FILE* file=fopen(filename,"r");

    if(file == NULL) return 0;

    fscanf(file, " %c %d", &command.var1, &commands[count].number);

    while(fscanf(file , "%s") == 1)
    {
        if(strcmp(opcode, "Mal") == 0)
        {
            command.op = MAL;
        }
        else if(strcmp(opcode, "Ass") == 0)
        {
            command.op = ASS;
        }
        else if(strcmp(opcode, "Inc") == 0)
        {
            commands[count].op = INC;
        }
        else if(strcmp(opcode, "Dec") == 0)
        {
            commands[count].op = DEC;
        }
        else if(strcmp(opcode, "Pri") == 0)
        {
            commands[count].op = PRI;
        }
        else if(strcmp(opcode, "Add") == 0)
        {
            commands[count].op = ADD;
        }
        else if(strcmp(opcode, "Sub") == 0)
        {
            commands[count].op = SUB;
        }
        else if(strcmp(opcode, "Mul") == 0)
        {
            commands[count].op = MUL;
        }
        else if(strcmp(opcode, "And") == 0)
        {
            commands[count].op = AND;
        }
        else if(strcmp(opcode, "Xor") == 0)
        {
            commands[count].op = XOR;
        }
        else if(strcmp(opcode, "Fre") == 0)
        {
            commands[count].op = FRE;
        }
        else if(strcmp(opcode, "Pra") == 0)
        {
            commands[count].op = PRA;
        }

        count ++;
    }

    fclose(file);

    return count;
}
