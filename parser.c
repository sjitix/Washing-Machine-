#include <stdio.h>
#include <string.h>
#include "parser.h"

struct internalCommand {
    int op;
    char var1;
    char var2;
    int number;
};

static struct internalCommand commands[100];

Command get_command(int index)
{
    return &commands[index];
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

    int count = 0;

    char opcode[4];

    while(fscanf(file , "%s", opcode) == 1)
    {
        if(strcmp(opcode, "Mal") == 0)
        {
            commands[count].op = MAL;
            fscanf(file, " %c %d", &commands[count].var1, &commands[count].number);
        }
        else if(strcmp(opcode, "Ass") == 0)
        {
            commands[count].op = ASS;
            fscanf(file, " %c %d", &commands[count].var1, &commands[count].number);
        }
        else if(strcmp(opcode, "Inc") == 0)
        {
            commands[count].op = INC;
            fscanf(file, " %c %d", &commands[count].var1, &commands[count].number);
        }
        else if(strcmp(opcode, "Dec") == 0)
        {
            commands[count].op = DEC;
            fscanf(file, " %c %d", &commands[count].var1, &commands[count].number);
        }
        else if(strcmp(opcode, "Pri") == 0)
        {
            commands[count].op = PRI;
            fscanf(file, " %c %d", &commands[count].var1, &commands[count].number);
        }
        else if(strcmp(opcode, "Add") == 0)
        {
            commands[count].op = ADD;
            fscanf(file, " %c %c", &commands[count].var1, &commands[count].var2);
        }
        else if(strcmp(opcode, "Sub") == 0)
        {
            commands[count].op = SUB;
            fscanf(file, " %c %c", &commands[count].var1, &commands[count].var2);
        }
        else if(strcmp(opcode, "Mul") == 0)
        {
            commands[count].op = MUL;
            fscanf(file, " %c %c", &commands[count].var1, &commands[count].var2);
        }
        else if(strcmp(opcode, "And") == 0)
        {
            commands[count].op = AND;
            fscanf(file, " %c %c", &commands[count].var1, &commands[count].var2);
        }
        else if(strcmp(opcode, "Xor") == 0)
        {
            commands[count].op = XOR;
            fscanf(file, " %c %c", &commands[count].var1, &commands[count].var2);
        }
        else if(strcmp(opcode, "Fre") == 0)
        {
            commands[count].op = FRE;
            fscanf(file, " %c", &commands[count].var1);
        }
        else if(strcmp(opcode, "Pra") == 0)
        {
            commands[count].op = PRA;
            fscanf(file, " %c", &commands[count].var1);
        }

        count ++;
    }

    fclose(file);

    return count;
}
