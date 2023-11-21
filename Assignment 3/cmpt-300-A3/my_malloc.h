#ifndef _MY_MALLOC_H_
#define _MY_MALLOC_H_

//header file for my_malloc.h
#include <stdio.h>
#include "list.h"
#define MALLOC_SIZE

//initially gets a chunk of memory from heap

//doesnt return anything
void mem_init(size_t bytes_req);

//input: n size of bytes 
//returns void pointer type 
void* my_malloc(size_t size);

//input: pointer, output: void
void my_free(void* ptr);


//additional helper functions 
void* find_fit(size_t size); // finds a free slot for the memory 
//coalesing (ie combining free memory to create a bigger chunk) -> O(1) time
#endif

//additional helper functions 
//find_fit() // finds a free slot for the memory 
//coalesing (ie combining free memory to create a bigger chunk) -> O(1) time