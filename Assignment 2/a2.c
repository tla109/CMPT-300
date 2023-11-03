#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd; // File descriptor
    char buffer[100]; // Buffer to store read data
    ssize_t bytesRead; // Number of bytes read

    // Open a file in read-only mode
    fd = open("example.txt", O_RDONLY);

    if (fd == -1) {
        perror("Error opening the file");
        exit(EXIT_FAILURE);
    }

    // Read data from the file descriptor into the buffer
    bytesRead = read(fd, buffer, sizeof(buffer));

    if (bytesRead == -1) {
        perror("Error reading from the file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Null-terminate the buffer to treat it as a string
    buffer[bytesRead] = '\0';

    // Print the read content
    printf("Read from the file: %s\n", buffer);

    // Close the file descriptor
    close(fd);

    return 0;
}
