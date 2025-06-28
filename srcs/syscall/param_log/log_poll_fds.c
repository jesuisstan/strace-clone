#define _GNU_SOURCE

#include "param_log.h"
#include <ft_printf.h>
#include <ft_strace_utils.h>
#include <macros.h>
#include <registers.h>
#include <sys/poll.h>
#include <sys/uio.h>

static const flag_str_t flags[] = {
	FLAG_STR(POLLIN),	  FLAG_STR(POLLPRI),	FLAG_STR(POLLOUT),	  FLAG_STR(POLLRDHUP),
	FLAG_STR(POLLERR),	  FLAG_STR(POLLHUP),	FLAG_STR(POLLNVAL),	  FLAG_STR(POLLRDNORM),
	FLAG_STR(POLLRDBAND), FLAG_STR(POLLWRNORM), FLAG_STR(POLLWRBAND),
};

static int log_poll_fd(struct pollfd *fds, size_t i)
{
	size_t size_written = 0;
	if (i != 0)
		size_written += ft_dprintf(STDERR_FILENO, ", ");
	size_written += ft_dprintf(STDERR_FILENO, "{fd=%d, events=", fds[i].fd);
	if (fds[i].events == 0)
	{
		size_written += ft_dprintf(STDERR_FILENO, "0");
		return size_written;
	}
	bool_t first = true;
	for (size_t j = 0; j < ELEM_COUNT(flags); j++)
	{
		if (fds[i].events & flags[j].flag)
		{
			if (!first)
				size_written += ft_dprintf(STDERR_FILENO, "|");
			size_written += ft_dprintf(STDERR_FILENO, "%s", flags[j].str);
			first = false;
			fds[i].events &= ~flags[j].flag;
		}
	}
	if (fds[i].events != 0)
	{
		if (!first)
			size_written += ft_dprintf(STDERR_FILENO, "|");
		size_written += ft_dprintf(STDERR_FILENO, "%#x", fds[i].events);
	}
	size_written += ft_dprintf(STDERR_FILENO, "}");
	return size_written;
}

/**
 * @brief Log poll fds
 *
 * @param value
 * @param context
 * @return int
 */
int log_POLL_FDS(uint64_t value, syscall_log_param_t *context)
{
	if (value == 0)
		return ft_dprintf(STDERR_FILENO, "NULL");
	uint64_t fd_count = registers_get_param(context->regs, context->type, context->arg_index + 1);
	if (fd_count == 0)
		return ft_dprintf(STDERR_FILENO, "[]");
	struct pollfd *fds = malloc(sizeof(struct pollfd) * fd_count);
	if (!fds)
	{
		log_error("log_POLL_FDS", "malloc failed", true);
		return 0;
	}
	if (remote_memcpy(fds, context->pid, (void *)value, sizeof(struct pollfd) * fd_count) < 0)
	{
		free(fds);
		return ft_dprintf(STDERR_FILENO, "%p", (void *)value);
	}
	int size_written = 0;
	size_written += ft_dprintf(STDERR_FILENO, "[");
	for (uint64_t i = 0; i < fd_count; i++)
		size_written += log_poll_fd(fds, i);
	size_written += ft_dprintf(STDERR_FILENO, "]");
	free(fds);
	return size_written;
}
