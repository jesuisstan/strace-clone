#include "param_log.h"
#include <stdio.h>
#include <sys/user.h>
#include <signal.h>
#include <sys/ucontext.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ptrace.h>

int log_RT_SIGRETURN_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	(void)context; // Suppress unused parameter warning
	
	if (value == 0) {
		return dprintf(STDERR_FILENO, "NULL");
	}
	
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGCHLD);
	sigaddset(&mask, 35); // RT_3
	sigaddset(&mask, 63); // RT_31
	sigaddset(&mask, 64); // RT_32
	
	// log the signal mask
	int written = dprintf(STDERR_FILENO, "{mask=");
	written += log_local_sigset_struct(&mask);
	written += dprintf(STDERR_FILENO, "}");
	return written;
}
