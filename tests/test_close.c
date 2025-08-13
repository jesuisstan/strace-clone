#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    // Test close with various file descriptors
    int fd1 = open("/dev/null", O_RDONLY);
    int fd2 = open("/dev/zero", O_RDONLY);
    int fd3 = open("/dev/urandom", O_RDONLY);
    
    printf("Opened fds: %d, %d, %d\n", fd1, fd2, fd3);
    
    // Close them
    close(fd1);
    close(fd2);
    close(fd3);
    
    // Test close with invalid fd
    close(999);
    
    // Test close with stdin/stdout/stderr
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    return 0;
}
