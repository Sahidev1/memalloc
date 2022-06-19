#include <unistd.h>
#include <stdio.h> // remove this later!!!

void* currBreak(){
    intptr_t zero = 0;
    return sbrk(zero);
}

//This is only here for testing, remove later
int main(){
    void* start_of_block;

    void* initBrk = currBreak(); // do not use this!!!

    printf("init break: %p \n", initBrk);

    intptr_t bytes = 24;
    void* ptr = sbrk(bytes);
    start_of_block = ptr; //always reassign start of block after sbrk!
    void* end_of_block = currBreak();

    printf ("break: %p \n", ptr);

    printf ("new break %p \n", currBreak());

    int* int_in_heap = (int*) (start_of_block);
    *int_in_heap = 44;
    int val = *int_in_heap;

    printf ("int heap addr: %p \n", int_in_heap);
    printf ("int_in heap val: %d \n", (*int_in_heap));

    int* init_out_bounds = (int*) (end_of_block + 1241);
    *init_out_bounds = 4918314;
    printf ("int heap addr: %p \n", init_out_bounds);
    printf ("int_in heap val: %d \n", (*init_out_bounds));

    void* negvoid = (void*) -1;
    printf ("zero pointer: %p \n", negvoid);

    __uint64_t diff = (__uint64_t) start_of_block;
    printf ("diff: %lu \n", diff);


    
    return 0;
}