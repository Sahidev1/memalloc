#include "memalloc.h"
#include <stdio.h>

void println (){
    printf("\n");
}

int main(){
    /*printf ("str addr: %p \n", get_end_of_block());

    void* mallptr = memalloc (32);
    printf ("addr: %p \n", mallptr);

    printf ("end addr: %p \n", get_end_of_block());

    Header* strt = get_start_header();
    Header* prev;
    while (strt != NULL){
        printf ("head addr: %p \n", (void* )strt);
        printf ("head val: %d \n", strt->info);
        prev = strt;
        strt = strt->next;
    }
    printf ("head addr: %p \n", (void* )prev);
    prev += 125000 / sizeof(Header);
    printf ("head addr: %p \n", (void* )prev);
    prev->info = 5;
    printf ("size of void: %li", sizeof(Header));
    return 0;
    */
   const int ARR_SIZE = 30;
   int* arr = (int*) memalloc(ARR_SIZE*sizeof(int));
   arr[5] = 33;

   printf ("arr addr: %p \n", (void*) arr);
   printf ("arr val index 5 %d \n", arr[5]);

   char* carr = (char*) memalloc (331*sizeof(char));
   carr[123] = 'h';

    printf ("arr addr: %p \n", (void*) carr);
   printf ("arr val index 123: %c \n", carr[123]);
}