#include <stdint.h>

#ifndef MEMALLOC_H_
#define MEMALLOC_H_

#define IS_ALLOCATED 1
#define IS_NOT_ALLOCATED 0

typedef enum {FALSE, TRUE} boolean;

typedef struct header{
    int info;
    struct header* next;
} Header;

void* get_end_of_block ();
void init_mem ();
void* memalloc (int bytes);
void set_error_flag(void* sbrk_ret_value);
void init_blocks ();

int generate_info (uint64_t size);
int get_size_from_info (int size);
int set_alloc_state_info (int info, int allocstate);
int get_alloc_state (int info);

// TEST FUNCTIONS
Header* get_start_header ();
void head_traverser ();

#endif