// file for malloc.c
#include "my_malloc.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
#define ALIGNMENT 8
// linux requires 8 byte alignment
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
// first bit is allocation ie if memory is in use or not
#define SIZE_T_SIZE (ALIGN(sizeof(size_t))
// header size
*/

void *memory_block;
List* free_list; // link list to manage free blocks

// attempting first fit algo
// reference for man pages -> https://man7.org/linux/man-pages/man3/malloc.3.html

// initially gets a chunk of memory from heap
// returns a pointer
void mem_init(size_t bytes_req) {
    // need to allocate the memory
    memory_block = malloc(bytes_req); //body of the memory
    //statically create the list 
    free_list = List_create();
    //then append the first one with the bytes req
    List_append(free_list, memory_block, bytes_req); //need to include size as well 
    
    if (memory_block == NULL) {
        printf("Memory not allocated.\n");
    exit(0);
    }
}
/*
from manual page
malloc()
       The malloc() function allocates size bytes and returns a pointer
       to the allocated memory.  The memory is not initialized.  If size
       is 0, then malloc() returns a unique pointer value that can later
       be successfully passed to free().
return a pointer to the allocated memory, which is suitably
   aligned for any type that fits into the requested size or less.

//input: n size of bytes
//returns void pointer type
void* my_malloc(size_t size) {
    printf("i am in my malloc\n");
    size_t blk_size = ALIGN(size + SIZE_T_SIZE); //size of block of mem
    size_t *header = find_fit(blk_size);
    if (header && blk_size < *header) {
        //split block if possible
        *(size_t *)((char *) header + blk_size) = *header - blk_size;
    } else {
        header = sbrk(blk_size); //sbrk changes space allocation
    }
    *header = blk_size | 1; //mark allocated bit
    return (char *)header + 8;
}

 from manual page
free()
   The free() function frees the memory space pointed to by ptr,
   which must have been returned by a previous call to malloc() or
   related functions.  Otherwise, or if ptr has already been freed,
   undefined behavior occurs.  If ptr is NULL, no operation is
   performed.
   The free() function returns no value, and preserves errno.


//input: pointer, output: void
void my_free(void* ptr) {
    printf("i am in my free\n");
    //base case -> if the ptr is already NULL just exit function
    if (ptr != NULL) {
        //free the memory + do stuff

    }

}

**heap_start() and heap_end() is a c thing; goes to where the start of the heap
is; so in this case you want to go to where the start of the list is

//helper function to find space on the list
void* find_fit(size_t size) {
    size_t *header = heap_start(); //need to search on the list get to the start
() while (header < head_end()) { if (!(*header & 1) && *header >= size)) return
header; header = (char *)header + (*header ^ ~1L);
    }
    return NULL;
}
*/