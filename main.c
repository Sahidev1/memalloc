#include "memalloc.h"
#include <stdio.h>

int main(){
    //printf ("str addr: %p \n", get_end_of_block());
    void* mallptr = memalloc (32);
    printf ("addr: %p \n", mallptr);

    printf ("end addr: %p \n", memalloc(22));
    printf ("end addr: %p \n", get_end_of_block());
    printf ("end addr: %p \n", get_end_of_block());

    Header* strt = get_start_header();
    printf("start val: %d \n", strt->size);

    printf("header ptr: %p \n", (void*) strt->next);
    return 0;
}