#include <stdio.h>
#include "memory.h"
#include "struct.h"

int parse(const char* filename, Command* commands);
void execute(Command cmd);

int main() {
    Command commands[1000];

    memory_init();

    int count = parse("test.txt", commands);

    for (int i = 0; i < count; i++) {
        execute(commands[i]);
    }

    return 0;
}
