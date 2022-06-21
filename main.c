#include "memalloc.h"
#include <stdio.h>

void test0 (){
    const int ARR_SIZE = 30;
   int* arr = (int*) memalloc(ARR_SIZE*sizeof(int));
   arr[5] = 33;

    Header* tmp = get_start_header ();
    printf ("head addr: %p \n", (void*) tmp);
    printf ("head size: %d \n", get_size_from_info(tmp->info));
    Header* hptr = (Header*)(((void*) arr) - sizeof(Header));
    printf ("head addr: %p \n", (void*) hptr);
    printf ("head size: %d \n", get_size_from_info(hptr->info));

   printf ("arr addr: %p \n", (void*) arr);
   printf ("arr val index 5 %d \n", arr[5]);

   char* carr = (char*) memalloc (331*sizeof(char));
   carr[123] = 'h';

    printf ("arr addr: %p \n", (void*) carr);
   printf ("arr val index 123: %c \n", carr[123]);

    long* larr = (long*) memalloc (2000*sizeof(long));
    printf ("arr addr: %p \n", (void*) larr);

    hptr = (Header*)(((void*) larr) - sizeof(Header));
    printf ("head addr: %p \n", (void*) hptr);
    printf ("head size: %d \n", get_size_from_info(hptr->info));

    head_traverser(922);
}

void test1 (){
    printf ("str addr: %p \n", get_end_of_block());

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
}

void test2 (){
    int traverse_depth = 4;
    int* intarr = (int*) memalloc(sizeof(int)*40);
    intarr[4] = 11;

    char* carr = (char*) memalloc(sizeof(char)*2048);
    carr[1123] = 'F';
    head_traverser(traverse_depth);

    free(intarr);
    head_traverser(traverse_depth);

    long* arr = (long*) memalloc(sizeof(long)*30);
    arr[3] = 2414;

    head_traverser(traverse_depth);

    int* iarr = (int*) memalloc(sizeof(int)*30);
    iarr[11] = 34;

    head_traverser(traverse_depth);

    free(iarr);
    head_traverser(traverse_depth);
}

int main(){
   test2();
}