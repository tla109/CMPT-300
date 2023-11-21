#include "my_malloc.h"
#include <stdio.h>

int main() {
    mem_init(50000); // Initialize with a 1024-byte memory block

    // Test your allocation and deallocation here
    void* ptr1 = my_malloc(10000);
    void* ptr2 = my_malloc(20000);

    if (ptr1 != NULL && ptr2 != NULL) {
        printf("Memory allocated successfully.\n");

        // Perform operations with allocated memory

        my_free(ptr1);
        my_free(ptr2);

        printf("Memory freed successfully.\n");
    } else {
        printf("Memory allocation failed.\n");
    }

    return 0;
}
