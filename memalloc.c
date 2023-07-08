#include "memalloc.h"
#include <sys/mman.h>

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
    char *ptr = (char *) addr;
    return (void*) (ptr + (ALLOC_OVERHEAD/sizeof(char)));
}

mem_block* usable_to_base_addr (void* usable_addr){
    char *ptr = (char*) usable_addr;
    return (mem_block*) (ptr - (ALLOC_OVERHEAD/sizeof(char)));
}

void* memalloc (size_t block_size){ 
    if (first_block == NULL){
        first_block = alloc_new_block(block_size);
        return usable_alloc_addr (first_block);
    }

    mem_block* curr_block = first_block;
    mem_block* last_prev_block = curr_block;
    while (curr_block != NULL){
        if (curr_block->is_free && curr_block->size >= block_size){
            curr_block->is_free = FALSE;
            return usable_alloc_addr(curr_block);
        }
        if (curr_block->next_block == NULL) last_prev_block = curr_block;
        curr_block = curr_block->next_block;
    }
    curr_block = alloc_new_block(block_size);
    last_prev_block->next_block = curr_block;
    return usable_alloc_addr(curr_block);
}

// this only exists for testing purposes!
mem_block* get_mem_list (){
    return first_block;
}

int memfree (void* usable_memaddr){
    mem_block* block = usable_to_base_addr(usable_memaddr);
    block->is_free = TRUE;
    return 0;
}