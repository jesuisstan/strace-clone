#define _GNU_SOURCE

#include "param_log.h"
#include <ft_printf.h>
#include <ft_strace_utils.h>
#include <macros.h>
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
		return ft_dprintf(STDERR_FILENO, "%p", remote_ptr);
	return ft_dprintf(STDERR_FILENO, "[%ld, %ld]", fds[0], fds[1]);
}