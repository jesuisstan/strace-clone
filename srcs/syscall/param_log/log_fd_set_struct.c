#define _GNU_SOURCE

#include "param_log.h"
#include <ft_printf.h>
#include <ft_strace_utils.h>
#include <macros.h>
#include <registers.h>
#include <sys/select.h>
#include <sys/uio.h>

/**
 * @brief Log a fd_set struct
 *
 * @param value the ptr to fd_set
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_FD_SET_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(fd_set, set);
	int size_written = ft_dprintf(STDERR_FILENO, "[");
	bool_t first = true;
	int nfds = registers_get_param(context->regs, context->type, 0);
	for (int i = 0; i < nfds; i++)
	{
		if (FD_ISSET(i, &set))
		{
			if (!first)
				size_written += ft_dprintf(STDERR_FILENO, ", ");
			size_written += ft_dprintf(STDERR_FILENO, "%d", i);
			first = false;
		}
	}
	size_written += ft_dprintf(STDERR_FILENO, "]");
	return size_written;
}