// this is for drafting functions and stuff

// for coalescing full memory cells (should be called in executor.h FRE case):
void compact(idk, freed_index) {
    // idk means maybe the memory array?
    // freed_index means the index of the variable that was just freed
    // next_free means the index of the next free cell
    // arr means the memory array
    for (int i=freed_index+1; i<next_free; i++)
        arr[i-1] = arr[i];
}