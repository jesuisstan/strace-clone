#define _GNU_SOURCE

#include "param_log.h"
#include <ft_strace_utils.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <stdbool.h>

/**
 * @brief Log wait status
 *
 * @param status the status to log
 * @return int the number of bytes written
 */
static int log_wait_status(int status)
{
	int size_written = 0;
	bool first = true;
	if (WIFEXITED(status))
	{
		size_written +=
			dprintf(STDERR_FILENO, "{WIFEXITED(s), WEXITSTATUS(s) == %d}", WEXITSTATUS(status));
		first = false;
	}
	if (WIFSIGNALED(status))
	{
		size_written += dprintf(STDERR_FILENO, "%s{WIFSIGNALED(s), WTERMSIG(s) == %d",
								   first ? "" : ", ", WTERMSIG(status));
		if (WCOREDUMP(status))
			size_written += dprintf(STDERR_FILENO, ", WCOREDUMP(s) == %d", WCOREDUMP(status));
		size_written += dprintf(STDERR_FILENO, "}");
		first = false;
	}
	if (WIFSTOPPED(status))
	{
		size_written += dprintf(STDERR_FILENO, "%s{WIFSTOPPED(s), WSTOPSIG(s) == %d}",
								   first ? "" : ", ", WSTOPSIG(status));
		first = false;
	}
	if (WIFCONTINUED(status))
		size_written += dprintf(STDERR_FILENO, "%s{WIFCONTINUED(s)}", first ? "" : ", ");
	return size_written;
}

/**
 * @brief Log wait status
 *
 * @param value the ptr to wait status
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_WAIT_STATUS(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(int, status);
	int size_written = 0;
	size_written += dprintf(STDERR_FILENO, "[");
	size_written += log_wait_status(status);
	size_written += dprintf(STDERR_FILENO, "]");
	return size_written;
}