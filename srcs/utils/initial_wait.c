#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ft_strace_utils.h>

/**
 * @brief Initial wait for the tracee
 *
 * @param pid the pid of the tracee
 * @param status the status of the tracee
 * @param flag the flag to pass to waitpid
 * @return int 0 if the call succeeded, -1 otherwise
 */
int initial_wait(pid_t pid, int *status, int flag)
{
	if (waitpid(pid, status, flag) == -1) {
		return -1;
	}
	return 0;
}