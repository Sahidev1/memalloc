#include <stdio.h>
#include "memalloc.h"

void mem_iterator (){
    mem_block* b = get_mem_list();
    while(b != NULL){
        printf("addr: %p\n", b);
        printf("isfree: %d\n", b->is_free);
        printf("next addr: %p\n", b->next_block);
        b = b->next_block;
    }
    if (b == NULL) printf("memblock is nullpointer\n");
}

int main(void){
    printf("hello world!\n");
    printf("sizeof struct: %lu", sizeof(mem_block));

    int* ptr = (int *) memalloc(sizeof(int) * 2);
    if (ptr == (void*)-1) printf("failed to map");
    printf("pointer: %p\n", ptr);
    *ptr = 22;
    printf("%d\n", ptr[0]);

    double *newptr = (double *) memalloc(sizeof(double) * 11);
    if (newptr == (void*) - 1) printf("failed to map");
    *newptr = 0.122;
    printf("%f\n", newptr[0]);

    memfree(ptr);

    char* cptr = (char *) memalloc(sizeof(char));
    *cptr = 'f';

    mem_iterator();

    int nr = 17;
    int** pptr = (int**) memalloc(sizeof(int*) * nr);
    for (int i = 0; i < nr; i++){
        *(pptr + i) = (int*) memalloc(sizeof(int)* (1 + (rand()%21)));
    }

    for(int i = 0; i < nr; i++){
        if (!(rand()%2)) memfree(*(pptr + i));
    }

    mem_iterator();

    return 0;
}