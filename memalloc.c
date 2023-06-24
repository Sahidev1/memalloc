#include "memalloc.h"
#include <sys/mman.h>
#include <stdio.h> // delete this, only for debugging.

#define ALLOC_OVERHEAD (sizeof(mem_block))

mem_block* first_block;


mem_block* alloc_new_block (size_t block_size) {
    void* memaddr = mmap(NULL, block_size + ALLOC_OVERHEAD, PROT_WRITE|PROT_READ , MAP_PRIVATE|MAP_ANON, -1, 0);
    if (memaddr == (void*) -1)return memaddr;
    mem_block* m = (mem_block*) memaddr;
    m->is_free = FALSE;
    m->size = block_size;
    m->next_block = (mem_block*) NULL;
    return m;
}

void* usable_alloc_addr (mem_block* addr){
    int *ptr = (int*) addr;
    return (void*) (ptr + (ALLOC_OVERHEAD/sizeof(int)));
}

mem_block* usable_to_base_addr (void* usable_addr){
    int *ptr = (int*) usable_addr;
    return (mem_block*) (ptr - (ALLOC_OVERHEAD/sizeof(int)));
}

void* memalloc (size_t block_size){ 
    if (first_block == NULL){
        printf("first is null\n");
        first_block = alloc_new_block(block_size);
        return usable_alloc_addr (first_block);
    }

    mem_block* curr_block = first_block;
    mem_block* last_prev_block = curr_block;
    while (curr_block != NULL){
        printf("iterating :)\n");
        printf("curr: %p\n", curr_block);
        if (curr_block->is_free && curr_block->size >= block_size){
            curr_block->is_free = FALSE;
            printf("free block found: %p\n", curr_block);
            return usable_alloc_addr(curr_block);
        }
        if (curr_block->next_block == NULL) last_prev_block = curr_block;
        curr_block = curr_block->next_block;
    }
    printf("outside of while\n");
    curr_block = alloc_new_block(block_size);
    last_prev_block->next_block = curr_block;
    printf ("%p\n", curr_block); //for debug
    return usable_alloc_addr(curr_block);
}

mem_block* get_mem_list (){
    return first_block;
}

int memfree (void* usable_memaddr){
    mem_block* block = usable_to_base_addr(usable_memaddr);
    printf("freed: %p\n", block);
    block->is_free = TRUE;
    return 0;
}