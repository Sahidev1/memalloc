#include "memalloc.h"
#include <sys/mman.h>
#include <pthread.h>
#include <string.h>

#define ALLOC_OVERHEAD (sizeof(mem_block))

mem_block* first_block;

pthread_mutex_t lock;
pthread_once_t once = PTHREAD_ONCE_INIT;
int lock_initialized = FALSE;

/**
 * This function initializes the mutex lock
*/
void lock_initializer_routine(){
    pthread_mutex_init(&lock, NULL);
}

/**
 * This function allocates space for a new memory block in the virtual memory. 
*/
mem_block* alloc_new_block (size_t block_size) {
    void* memaddr = mmap(NULL, block_size + ALLOC_OVERHEAD, PROT_WRITE|PROT_READ , MAP_PRIVATE|MAP_ANON, -1, 0);
    if (memaddr == (void*) -1)return memaddr;
    mem_block* m = (mem_block*) memaddr;
    m->is_free = FALSE;
    m->size = block_size;
    m->next_block = (mem_block*) NULL;
    return m;
}

/**
 * This function takes a pointer to a memory block struct and returns
 * the start address of the allocated space the user requested.
*/
void* usable_alloc_addr (mem_block* addr){
    char *ptr = (char *) addr;
    return (void*) (ptr + (ALLOC_OVERHEAD/sizeof(char)));
}

/*
    This function takes the starting address of usable space within
    the memory block struct and returns that base address of the struct.
*/
mem_block* usable_to_base_addr (void* usable_addr){
    char *ptr = (char*) usable_addr;
    return (mem_block*) (ptr - (ALLOC_OVERHEAD/sizeof(char)));
}

/**
 * This is the function users call to allocated dynamic memory. 
 * The function is thread-safe. 
 * 
 * If no memory block structs have been initialized we create one
 * and assign $first_block global variable to it.
 * 
 * We iterate through the memory block linked-list to try and find a free
 * memory block that has enough space for the requested allocation size by the user.
 * 
 * If no free or appropiate sized memory block is found we create a new memory block. 
*/
void* alloc (size_t block_size, alloc_type type){ 
    pthread_once(&once, &lock_initializer_routine);
    pthread_mutex_lock(&lock);

    if (first_block == NULL){
        first_block = alloc_new_block(block_size);
        pthread_mutex_unlock(&lock);
        return usable_alloc_addr(first_block);
    }

    mem_block* curr_block = first_block;
    mem_block* last_prev_block = curr_block;
    void* usable;
    while (curr_block != NULL){
        if (curr_block->is_free && curr_block->size >= block_size){
            curr_block->is_free = FALSE;
            usable = usable_alloc_addr(curr_block);
            if(type == CALLOC){
                memset(usable,0,block_size);
            }
            pthread_mutex_unlock(&lock);
            return usable;
        }
        if (curr_block->next_block == NULL) last_prev_block = curr_block;
        curr_block = curr_block->next_block;
    }
    curr_block = alloc_new_block(block_size);
    last_prev_block->next_block = curr_block;
    pthread_mutex_unlock(&lock);
    return usable_alloc_addr(curr_block);
}

void* memalloc (size_t block_size) {
    return alloc(block_size, MEMALLOC);
}

void* mecalloc (size_t n_elems, size_t elem_size){
    return alloc(n_elems * elem_size, CALLOC);
}

// this only exists for testing purposes!
mem_block* get_mem_list (){
    return first_block;
}

/**
 * This function frees an existing memory block so
 * that it can be used for future allocation. 
*/
int memfree (void* usable_memaddr){
    pthread_mutex_lock(&lock);

    mem_block* block = usable_to_base_addr(usable_memaddr);
    block->is_free = TRUE;
    pthread_mutex_unlock(&lock);
    return 0;
}