#include <stdint.h>
#include <unistd.h>
#include "memalloc.h"
#include <stdio.h> // for debugging

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
    Header* tmpPtr = start_header;
    int isAlloced;
    int available_mem;
    int bytenr = (int) bytes;
    while (tmpPtr != NULL){
        isAlloced = get_alloc_state(tmpPtr->info);
        available_mem = get_size_from_info(tmpPtr->info) - sizeof(Header);
        if (isAlloced == IS_NOT_ALLOCATED && bytenr <= available_mem){
            tmpPtr->info = set_alloc_state_info(tmpPtr->info, IS_ALLOCATED);
            return (void*) (tmpPtr + sizeof(Header));
        }
        tmpPtr = tmpPtr->next;
    }
    return NULL;
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
    start_header->info = generate_info((uint64_t) (diff / NUMBER_OF_BLOCKS));
    Header* tmp = start_header;

    for (int i = 1; i < NUMBER_OF_BLOCKS; i++){
        tmp->next = (Header*) (start_of_block + i * (diff/NUMBER_OF_BLOCKS));
        tmp = tmp->next;
        tmp->info = generate_info(diff /NUMBER_OF_BLOCKS);
    }
    tmp->next = NULL;
}

//make sure size is less than 2^27 - 1!
int generate_info (uint64_t size){
    int intver = (int) size;
    return intver << 3;
}

int get_size_from_info (int size){
    return size >> 3;
}

int set_alloc_state_info (int info, int allocstate){
    if (allocstate == IS_ALLOCATED){
        info = (info & 0xFFFFFFF8) | 0x1;
    }
    else if (allocstate == IS_NOT_ALLOCATED) {
        info &= 0xFFFFFFF8;
    }
    return info;
}

int get_alloc_state (int info){
    return info & ~(0xFFFFFFF8);
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

