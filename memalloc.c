#include <stdint.h>
#include <unistd.h>
#include "memalloc.h"
#include <stdio.h> // for debugging

#define LOG2_LARGE_BLOCK_SIZE 30
#define LOG2_MAX_SUB_BLOCK_SIZE  29
#define LOG2_MIN_SUB_BLOCK_SIZE 10

const void* ERROR_SBRK_VALUE = (void*) -1;
const __intptr_t LARGE_BLOCK_SIZE = 0x1 << LOG2_LARGE_BLOCK_SIZE;
const int MAX_SUB_BLOCK_SIZE = 0x1 << LOG2_MAX_SUB_BLOCK_SIZE;
const int NUMBER_OF_BLOCKS = (LOG2_LARGE_BLOCK_SIZE - LOG2_MIN_SUB_BLOCK_SIZE) + 1;
const int HEADER_SIZE = sizeof(Header);

boolean first_malloc_call = TRUE;
void* start_of_block;
void* end_of_block;

Header* start_header;

boolean block_alloc_error_flag; //IF ERROR THEN TRUE

/**
 * @brief Allocates memory on the heap.
 * if first memalloc call initializes increases size of process heap to allow allocation.
 * This function allocates a block of the process heap that is free.
 * 
 * @param alloc_size bytes to allocate on the process heap
 * @return void* pointer to allocted memory on the heap
 */
void* memalloc (int alloc_size){
    if(first_malloc_call){
        init_mem();
    }
    Header* tmpPtr = start_header;
    int is_alloced;
    int available_mem;
    while (tmpPtr != NULL){
        is_alloced = get_alloc_state(tmpPtr->info);
        available_mem = get_size_from_info(tmpPtr->info) - HEADER_SIZE;
        if (is_alloced == IS_NOT_ALLOCATED && alloc_size <= available_mem){
            tmpPtr->info = set_alloc_state_info(tmpPtr->info, IS_ALLOCATED);
            return ((void*) tmpPtr) + HEADER_SIZE;
        }
        tmpPtr = tmpPtr->next;
    }
    return NULL;
}

/**
 * @brief Frees the memory that was allocated to the pointer ptr on the heap.
 * 
 * @param ptr pointer to allocated memory on the heap.
 */
void free (void* ptr){
    Header* ptr_head = (Header*) (ptr - HEADER_SIZE);
    ptr_head->info = set_alloc_state_info(ptr_head->info, IS_NOT_ALLOCATED);
}

/**
 * @brief calls sbrk to increase size of process heap, allowing free heap space that can be
 * allocated.
 * 
 */
void init_mem (){
    start_of_block = sbrk(LARGE_BLOCK_SIZE);
    set_error_flag(start_of_block);
    if(!block_alloc_error_flag){
        first_malloc_call = FALSE;
        end_of_block = sbrk(0);
        init_blocks();
    }
}

/**
 * @brief Initializes a number of blocks on the free heap space. Each block has a header 
 * with information about the size of the block, if the block is allocated, and a pointer to the next
 * blocks header.
 */
void init_blocks (){
    start_header = (Header*) start_of_block;
    int block_size = 0x1 << 10;

    start_header->info = generate_info(block_size);
    Header* tmp = start_header;
    tmp->next = (Header*) (start_of_block + block_size);
    tmp = tmp->next;
    tmp->info = generate_info(block_size);

    for (int i = 0; i < NUMBER_OF_BLOCKS - 2; i++){
        tmp->next = (Header*) (((void*)tmp) + (block_size));
        block_size <<= 1;
        tmp = tmp->next;
        tmp->info = generate_info(block_size);
    }
    tmp->next = NULL;
}

//generates an info variable with size set
int generate_info (uint64_t size){
    return ((int) size) << 1;
}

// decodes size from info variable
int get_size_from_info (int size){
    return size >> 1;
}

// Sets allocation state of info variable
int set_alloc_state_info (int info, int allocstate){
    if (allocstate == IS_ALLOCATED){
        info = (info & ~(0x1)) | 0x1;
    }
    else if (allocstate == IS_NOT_ALLOCATED) {
        info &= ~(0x1);
    }
    return info;
}

// returns allocation state of info variable
int get_alloc_state (int info){
    return info & 0x1;
}

// sets sbrk error flag, if sbrk fails error flag is set to TRUE
void set_error_flag(void* sbrk_ret_value){
    block_alloc_error_flag = sbrk_ret_value == ERROR_SBRK_VALUE;
}

// returns address of brk of heap
void* get_end_of_block (){
    return end_of_block;
}


// TESTING PURPOSE FUNCTIONS, REMOVE LATER! //

Header* get_start_header (){
    return start_header;
}

void head_traverser (int blocks_to_traverse){
    Header* tmp = start_header;
    int address_additive = 0;
    int incrementer = 0x1 << 10;
    int blockcounter = 1;
    boolean first_iter = TRUE;
    printf ("----------------------------HEADER TRAVERSER------------------------------------- \n");
    while (tmp != NULL && blocks_to_traverse > 0){
        printf("Header adress: %p \n", (void*) tmp);
        printf("Header size: %d \n", get_size_from_info(tmp->info));
        printf("Header alloc status: %d \n", get_alloc_state(tmp->info));
        printf("expected added sum to address so far: %d \n", address_additive);
        printf("block counter: %d \n", blockcounter);
        printf("\n");
        tmp = tmp->next;
        address_additive += incrementer;

        incrementer = first_iter?incrementer:(incrementer<<1);
        first_iter = first_iter?FALSE:first_iter;
        blocks_to_traverse--;
        blockcounter++;
    }
}
