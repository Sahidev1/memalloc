#include <stdint.h>
#include <unistd.h>
#include "memalloc.h"

const void* ERROR_SBRK_VALUE = (void*) -1;
const __intptr_t LARGE_BLOCK_SIZE = 4000000;
const uint32_t NUMBER_OF_BLOCKS = 32;
const int HEADER_SIZE = sizeof(Header);

boolean first_malloc_call = TRUE;
void* start_of_block;
void* end_of_block;

Header* start_header;

boolean block_alloc_error_flag; //IF ERROR THEN TRUE

void* memalloc (uint32_t bytes){
    if(first_malloc_call){
        init_mem();
    }
    return start_of_block;
}

void init_mem (){
    start_of_block = sbrk(LARGE_BLOCK_SIZE);
    set_error_flag(start_of_block);
    if(!block_alloc_error_flag){
        first_malloc_call = FALSE;
        end_of_block = sbrk(0);
        init_blocks();
    }
}

void init_blocks (){
    uint64_t start = (uint64_t) start_of_block;
    uint64_t end = (uint64_t) end_of_block;
    uint64_t diff = end - start;

    start_header = (Header*) start_of_block;
    start_header->size = diff / NUMBER_OF_BLOCKS;
    start_header->next == NULL;
}

void set_error_flag(void* sbrk_ret_value){
    block_alloc_error_flag = sbrk_ret_value == ERROR_SBRK_VALUE;
}

void* get_end_of_block (){
    return end_of_block;
}


// TESTING PURPOSE FUNCTIONS, REMOVE LATER!
Header* get_start_header (){
    return start_header;
}

