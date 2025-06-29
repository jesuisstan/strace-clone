#define _GNU_SOURCE

#include "param_log.h"
#include <ft_strace_utils.h>
#include <sys/poll.h>
#include <sys/uio.h>
#include <stdbool.h>

static int64_t registers_get_param(void *regs __attribute__((unused)), int type __attribute__((unused)), int index __attribute__((unused))) { return 0; }

static const flag_str_t flags[] = {
	FLAG_STR(POLLIN),	  FLAG_STR(POLLPRI),	FLAG_STR(POLLOUT),	  FLAG_STR(POLLRDHUP),
	FLAG_STR(POLLERR),	  FLAG_STR(POLLHUP),	FLAG_STR(POLLNVAL),	  FLAG_STR(POLLRDNORM),
	FLAG_STR(POLLRDBAND), FLAG_STR(POLLWRNORM), FLAG_STR(POLLWRBAND),
};

static int log_poll_fd(struct pollfd *fds, size_t i)
{
	size_t size_written = 0;
	if (fds[i].revents == 0)
		return size_written;
	if (i != 0)
		size_written += dprintf(STDERR_FILENO, ", ");
	size_written += dprintf(STDERR_FILENO, "{fd=%d, revents=", fds[i].fd);
	bool first = true;
	for (size_t j = 0; j < sizeof(flags) / sizeof(flags[0]); j++)
	{
		if (fds[i].revents & flags[j].flag)
		{
			if (!first)
				size_written += dprintf(STDERR_FILENO, "|");
			size_written += dprintf(STDERR_FILENO, "%s", flags[j].str);
			first = false;
			fds[i].revents &= ~flags[j].flag;
		}
	}
	if (fds[i].revents != 0)
	{
		if (!first)
			size_written += dprintf(STDERR_FILENO, "|");
		size_written += dprintf(STDERR_FILENO, "%#x", fds[i].revents);
	}
	size_written += dprintf(STDERR_FILENO, "}");
	return size_written;
}

/**
 * @brief Log poll fds
 *
 * @param value
 * @param context
 * @return int
 */
int log_POLL_FDS_AFTER(uint64_t value, syscall_log_param_t *context)
{
	int size_written = 0;
	size_written += dprintf(STDERR_FILENO, "%d", (int)value);
	if (value == 0)
	{
		size_written += dprintf(STDERR_FILENO, " (Timeout)");
		return size_written;
	}
	struct pollfd *fds = malloc(sizeof(struct pollfd) * value);
	if (!fds)
	{
		fprintf(stderr, "malloc failed");
		return 0;
	}
	unsigned int fd_count = registers_get_param(context->regs, context->type, 1);
	void *remote = (void *)registers_get_param(context->regs, context->type, 0);
	if (remote_memcpy(fds, context->pid, remote, sizeof(struct pollfd) * fd_count) < 0)
	{
		size_written += dprintf(STDERR_FILENO, "%p", remote);
		free(fds);
		return 0;
	}
	size_written += dprintf(STDERR_FILENO, " [{");
	for (size_t i = 0; i < fd_count; i++)
		size_written += log_poll_fd(fds, i);
	size_written += dprintf(STDERR_FILENO, "}]");
	free(fds);
	return size_written;
}