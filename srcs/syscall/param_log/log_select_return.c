#define _GNU_SOURCE

#include "param_log.h"
#include <fcntl.h>
#include <ft_printf.h>
#include <macros.h>
#include <sys/stat.h>
#include <sys/types.h>

/**
 * @brief Log select return
 *
 * @param value return value
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_SELECT_RETURN(uint64_t value, syscall_log_param_t *context)
{
	int64_t ret = (int64_t)value;
	int size_written = 0;
	size_written += ft_dprintf(STDERR_FILENO, "%ld ", ret);
	if (ret < 0)
		return size_written;
	uint64_t out_ptr = registers_get_param(context->regs, context->type, 2);
	if (out_ptr == 0)
	{
		uint64_t in_ptr = registers_get_param(context->regs, context->type, 1);
		if (in_ptr == 0)
			return size_written;
		size_written += ft_dprintf(STDERR_FILENO, "(in ");
		size_written += log_FD_SET_STRUCT(in_ptr, context);
	}
	else
	{
		size_written += ft_dprintf(STDERR_FILENO, "(out ");
		log_FD_SET_STRUCT(out_ptr, context);
	}
	size_written += ft_dprintf(STDERR_FILENO, ", left ");
	uint64_t left_ptr = registers_get_param(context->regs, context->type, 4);
	size_written += log_TIMEVAL_STRUCT(left_ptr, context);
	size_written += ft_dprintf(STDERR_FILENO, ")");
	return size_written;
}