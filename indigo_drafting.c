// this is for me to draft functions and stuff

// for coalescing full memory cells (should be called in executor.h FRE case):
void compact(idk, freed_index) {
    // idk means maybe the memory array?
    // freed_index means the index of the variable that was just freed
    // next_free means the index of the next free cell
    // arr means the memory array
    for (int i=freed_index+1; i<next_free; i++)
        arr[i-1] = arr[i];
}

// THIS DOES NOT WORK, IGNORE
// allocates space for variable in memory and labels it
int memory_allocate(int size, char name) {
    // uses global variable next_free
    // size is MAL cmd.b
    // name is MAL cmd.a
    // called in executor.h MAL
    name = next_free; // BROKEN, RELIES ON DYNAMICS VARIABLE DECLARATION
    next_free += size;
}

// alternative ADD command case
case ADD:
    x = cmd.a;
    y = cmd.b;
    xVal = memory_read(var_get_start(x));
    yVal = memory_read(var_get_start(y));
    sum = xVal + yVal;
    memory_write(sum, var_get_start(x));
    break;

// executor.c test function
void test_execute(void) {
	Command cmd;
	cmd.op = 
	cmd.var1 = 
	cmd.var2 = 
	execute(cmd);
	printf("Desired result: \n"); // write in what it's actually supposed to return
	printf("Actual result: \n", ); // write in whatever it is returning
	// repeat for all possible cmd.op values
}
int main(void) {
	test_execute
