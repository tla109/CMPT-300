#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "my_malloc.h"


int main(void) {
    printf("in mem_test\n");
    //start off by requesting a large chunk of memory, then subsequent requests are satisfied by the large chunk given 
    size_t bytes_req;
    int result = 0;

    printf("Please enter the number of bytes you want: ");
    result = scanf("%zu", &bytes_req);
    if (result) {
        printf("You requested %zu bytes\n", bytes_req);
        mem_init(bytes_req);
        //char* mem_ptr = mem_init(bytes_req);
        //printf("the memory ptr %p\n", mem_ptr);
    } else {
        printf("invalid input\n");
    }









  return 0;
}