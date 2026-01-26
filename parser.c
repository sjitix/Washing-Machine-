// TO DO: replace "file" with actual file name?

#include <stdio.h>
#include <stdlib.h>
#include "declarations.h"

//might change file open to be in main.c?
FILE* file=fopen(fileName,"r");
if (file == NULL) {
    printf("No such file.\n");
    return 0;
}

Command cmd;
while (fscanf(file,"%p %c %c", cmd.op, cmd.a, cmd.b) == 1)
    execute(cmd);

fclose(file);