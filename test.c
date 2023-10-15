#include <stdio.h>
#include "memalloc.h"
#include <time.h>

void mem_iterator (){
    mem_block* b = get_mem_list();
    while(b != NULL){
        printf("addr: %p\n", b);
        printf("isfree: %d\n", b->is_free);
        printf("size %ld\n",b->size);
        printf("next addr: %p\n", b->next_block);
        b = b->next_block;
    }
    if (b == NULL) printf("memblock is nullpointer\n");
}

int main(void){

    // allocating dynamic memory
    int* ptr = (int *) memalloc(sizeof(int) * 2);
    if (ptr == (void*)-1) printf("failed to map");
    printf("pointer addr: %p\n", ptr);
    *ptr = 22;
    printf("pointer value: %d\n", ptr[0]);

    // allocating dynamic memory again
    double *newptr = (double *) memalloc(sizeof(double) * 11);
    if (newptr == (void*) - 1) printf("failed to map");
    *newptr = 0.122;
    printf("%f\n", newptr[0]);

    memfree(ptr); // freeing first allocation

    // allocating memory for a char, the previously freed block should 
    // get allocated to this char pointer
    char* cptr = (char *) memalloc(sizeof(char));  
    printf("pointer addr: %p\n", cptr);
    *cptr = 'f';

    //mem_iterator();

    //bellow we allocate memory for an array of pointers
    //each pointer in the array is then allocated memory to an array of integers
    srand(time(NULL));
    int nr = 6;
    int** pptr = (int**) memalloc(sizeof(int*) * nr);
    for (int i = 0; i < nr; i++){
        *(pptr + i) = (int*) memalloc(sizeof(int)* (1 + (rand()%21)));
    }
    
    printf("before freeing:\n\n");
    mem_iterator();

    //we randomly free certain memory blocks
    for(int i = 0; i < nr; i++){
        if (!(rand()%2)) memfree(*(pptr + i));
    }

    printf("after freeing:\n\n");
    mem_iterator();

    //test
    int* arr2 = memalloc(51*sizeof(int));
    for(int i = 0; i < 51; i++){
        arr2[i] = rand();
        printf("malloc val: %d\n", arr2[i]);
        printf("malloc adr: %p\n", (arr2 + i));
    };

    memfree(arr2);

    //test mecalloc 
    int* arr0 = mecalloc(50, sizeof(int));
    for(int i = 0; i < 50; i++){
        printf("mecalloc val: %d\n", arr0[i]);
        printf("mecalloc adr: %p\n", (arr0 + i));
    };
    mem_iterator();
    memfree(arr0);
    mem_iterator();

    return 0;
}