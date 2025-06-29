#define _GNU_SOURCE

#include "param_log.h"
#include <ft_strace_utils.h>
#include <sys/uio.h>
#include <unistd.h>

/**
 * @brief Log pipe fds
 *
 * @param value the ptr to fds
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_PIPEFDS(uint64_t value, syscall_log_param_t *context)
{
	int size_written = 0;
	void *remote_ptr = handle_ptr(value, context, &size_written);
	if (remote_ptr == NULL)
		return size_written;
	int fds[2];
	if (remote_memcpy(fds, context->pid, remote_ptr, sizeof(fds)) < 0)
		return dprintf(STDERR_FILENO, "%p", remote_ptr);
	return dprintf(STDERR_FILENO, "[%d, %d]", fds[0], fds[1]);
}