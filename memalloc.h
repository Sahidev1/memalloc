#include <stdint.h>

#ifndef MEMALLOC_H_
#define MEMALLOC_H_

typedef enum {FALSE, TRUE} boolean;

typedef struct header{
    int size;
    struct header* next;
} Header;

void* get_end_of_block ();
void init_mem ();
void* memalloc (uint32_t bytes);
void set_error_flag(void* sbrk_ret_value);
void init_blocks ();

// TEST FUNCTIONS
Header* get_start_header ();

#endif