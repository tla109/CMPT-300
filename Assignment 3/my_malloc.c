#include "my_malloc.h"
#include <stdlib.h> // for system memory allocation
#include <stdint.h> //for uintptr_t
#include <stdio.h>

#define ALIGNMENT 8
#define MAX_ALLOCATIONS 100

// Round up to the nearest multiple of ALIGNMENT
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

// Define a structure for a free block
typedef struct block {
    size_t size;
    struct block* next;
} __attribute__((aligned(ALIGNMENT)))Block;

// Define the number of free lists (adjust as needed)
#define NUM_FREE_LISTS 10

// Data structure to track allocated blocks
typedef struct {
    void* ptr;
    size_t size;
} AllocationInfo;

// Array of free lists
static Block* free_lists[NUM_FREE_LISTS] = {NULL};

// Pointer to the start of the allocated memory block
static void* allocated_block_start = NULL;

// Helper function to find the appropriate free list index
static int get_free_list_index(size_t size) {
    // Adjust this function based on your block size ranges
    // For simplicity, dividing by 16 is used as an example
    return (size / 16) % NUM_FREE_LISTS;
}

void* my_malloc(size_t size) {
    int list_index = get_free_list_index(size);

    Block* current = free_lists[list_index];
    Block* previous = NULL;

    // Find a suitable free block
    while (current != NULL) {
        if (current->size >= size) {
            // Remove the block from the free list
            if (previous != NULL) {
                previous->next = current->next;
            } else {
                free_lists[list_index] = current->next;
            }

            // Ensure user-facing memory is aligned
            return (void*)((uintptr_t)((char*)current + sizeof(Block) + ALIGNMENT - 1) & ~(ALIGNMENT - 1));
        }

        previous = current;
        current = current->next;
    }

    // If no suitable block is found, request additional memory
    size_t block_size = sizeof(Block) + size;
    current = (Block*)malloc(block_size);

    if (current != NULL) {
        current->size = block_size;
        return (void*)((uintptr_t)((char*)current + sizeof(Block) + ALIGNMENT - 1) & ~(ALIGNMENT - 1));
    } else {
        return NULL; // Memory allocation failed
    }
}

void my_free(void *ptr) {
    // Mark the block as free and add it to the appropriate free list
    if (ptr != NULL) {
        Block* block = (Block*)((char*)ptr - sizeof(Block));
        int list_index = get_free_list_index(block->size - sizeof(Block));

        // Add the block to the beginning of the free list
        block->next = free_lists[list_index];
        free_lists[list_index] = block;
    }
}

void mem_init(size_t size) {
    // Request a large contiguous block of memory from the operating system
    allocated_block_start = malloc(size);

    if (allocated_block_start != NULL) {
        // Divide the block into segments for each free list
        size_t segment_size = size / NUM_FREE_LISTS;
        for (int i = 0; i < NUM_FREE_LISTS; ++i) {
            free_lists[i] = (Block*)((char*)allocated_block_start + i * segment_size);
            free_lists[i]->size = segment_size;
            free_lists[i]->next = NULL;
        }
    }
}
