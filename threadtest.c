#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "memalloc.h"

#define NUM_THREADS 40
#define MAX_ALLOCS 100
#define BLOCK_SIZE 128

void* thread_func(void* arg) {
    srand(time(NULL));
    int i;
    int allocs = 1 + rand()%MAX_ALLOCS;
    void* blocks[allocs];

    // allocate memory blocks
    for (i = 0; i < allocs; i++) {
        blocks[i] = memalloc(BLOCK_SIZE);
        if (blocks[i] == NULL) {
            printf("memalloc failed\n");
            exit(1);
        }
    }

    // free memory blocks
    for (i = 0; i < allocs; i++) {
        memfree(blocks[i]);
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int i;

    // create threads
    for (i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, thread_func, NULL) != 0) {
            printf("pthread_create failed\n");
            return 1;
        }
    }

    // wait for threads to finish
    for (i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            printf("pthread_join failed\n");
            return 1;
        }
    }

    printf("Test passed succesfully\n")
    return 0;
}
