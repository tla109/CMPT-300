#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include <stddef.h>

void* my_malloc(size_t size);
void my_free(void *ptr);
void mem_init(size_t size);

#endif /* MY_MALLOC_H */
