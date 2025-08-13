#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t mypid = getpid();
    
    printf("My PID: %d\n", mypid);
    
    // Test kill with various signals
    kill(mypid, SIGUSR1);
    kill(mypid, SIGUSR2);
    kill(mypid, SIGTERM);
    
    // Test kill with invalid PID
    kill(999999, SIGTERM);
    
    // Test kill with invalid signal
    kill(mypid, 999);
    
    // Test kill with 0 (check if process exists)
    kill(1, 0);  // init process should exist
    kill(999999, 0);  // invalid process
    
    return 0;
}
