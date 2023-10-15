#ifndef MEMALLOC_H
#define MEMALLOC_H

#include <stdlib.h>

#define TRUE 1
#define FALSE 0

//FLAGS

typedef enum {MEMALLOC, CALLOC, REALLOC} alloc_type;

typedef struct Mem_block {
    int is_free;
    size_t size;
    struct Mem_block* next_block;
} mem_block;

void* memalloc (size_t block_size);
void* mecalloc (size_t n_elems, size_t elem_size);
int memfree (void* usable_memaddr);

// below are for testing purposes
mem_block* alloc_new_block (size_t block_size);
mem_block* get_mem_list();

#endif