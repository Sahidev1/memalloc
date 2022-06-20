#include <stdint.h>
#include <unistd.h>
#include "memalloc.h"
#include <stdio.h> // for debugging

const void* ERROR_SBRK_VALUE = (void*) -1;
const __intptr_t LARGE_BLOCK_SIZE = (0x1 << 30);
const int MAX_BLOCK_SIZE = 0x1 << 29;
const int NUMBER_OF_BLOCKS = 29 - 10;
const int HEADER_SIZE = sizeof(Header);

boolean first_malloc_call = TRUE;
void* start_of_block;
void* end_of_block;

Header* start_header;

boolean block_alloc_error_flag; //IF ERROR THEN TRUE

void* memalloc (int bytes){
    if(first_malloc_call){
        init_mem();
    }
    Header* tmpPtr = start_header;
    int is_alloced;
    int available_mem;
    while (tmpPtr != NULL){
        is_alloced = get_alloc_state(tmpPtr->info);
        available_mem = get_size_from_info(tmpPtr->info) - sizeof(Header);
        if (is_alloced == IS_NOT_ALLOCATED && bytes <= available_mem){
            tmpPtr->info = set_alloc_state_info(tmpPtr->info, IS_ALLOCATED);
            return ((void*) tmpPtr) + sizeof(Header);
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
    start_header = (Header*) start_of_block;
    int block_size = 0x1 << 10;
    start_header->info = generate_info(block_size);
    Header* tmp = start_header;

    for (int i = 0; i < NUMBER_OF_BLOCKS; i++){
        tmp->next = (Header*) (((void*)tmp) + (block_size << i));
        tmp = tmp->next;
        tmp->info = generate_info(block_size << (i + 1));
    }
    tmp->next = NULL;
}

//make sure size is less than 2^30 - 1!
int generate_info (uint64_t size){
    return ((int) size) << 1;
}

int get_size_from_info (int size){
    return size >> 1;
}

int set_alloc_state_info (int info, int allocstate){
    if (allocstate == IS_ALLOCATED){
        info = (info & ~(0x1)) | 0x1;
    }
    else if (allocstate == IS_NOT_ALLOCATED) {
        info &= ~(0x1);
    }
    return info;
}

int get_alloc_state (int info){
    return info & 0x1;
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

void head_traverser (){
    Header* tmp = start_header;
    int address_additive = 0;
    int incrementer = 0x1 << 10;
    printf ("----------------------------HEADER TRAVERSER------------------------------------- \n");
    while (tmp != NULL){
        printf("Header adress: %p \n", (void*) tmp);
        printf("Header size: %d \n", get_size_from_info(tmp->info));
        printf("Header alloc status: %d \n", get_alloc_state(tmp->info));
        printf("expected added sum to address so far: %d \n", address_additive);
        tmp = tmp->next;
        address_additive += incrementer;
        incrementer = incrementer << 1;
    }
}
