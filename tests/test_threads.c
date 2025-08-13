#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define THREADS 3

void* thread_func(void* arg) {
    int num = *(int*)arg;
    char filename[32];
    snprintf(filename, sizeof(filename), "thread_%d.txt", num);

    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        return NULL;
    }
    char buf[64];
    snprintf(buf, sizeof(buf), "Hello from thread %d\n", num);
    write(fd, buf, strlen(buf));
    close(fd);
    sleep(1);
    return NULL;
}

int main() {
    pthread_t threads[THREADS];
    int nums[THREADS];
    for (int i = 0; i < THREADS; ++i) {
        nums[i] = i + 1;
        pthread_create(&threads[i], NULL, thread_func, &nums[i]);
    }
    for (int i = 0; i < THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }
    printf("All threads finished\n");
    return 0;
}