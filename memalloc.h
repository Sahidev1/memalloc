#ifndef MEMALLOC_H
#define MEMALLOC_H

#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef struct Mem_block {
    int is_free;
    size_t size;
    struct Mem_block* next_block;
} mem_block;

mem_block* alloc_new_block (size_t block_size);
void* memalloc (size_t block_size);
int memfree (void* usable_memaddr);
mem_block* get_mem_list();

#endif