#include "param_log.h"
#include <ft_printf.h>
#include <ft_strace_utils.h>
#include <macros.h>
#include <registers.h>
#include <sys/uio.h>
#include <unistd.h>

/**
 * @brief Log a int array
 *
 * @param value the pointer to the array
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_INT_ARRAY(uint64_t value, syscall_log_param_t *context)
{
	int size_written = 0;
	if (value == 0)
		return ft_dprintf(STDERR_FILENO, "NULL");
	int64_t array_size = REGISTERS_GET_RETURN(context->regs, context->type);
	if (array_size < 0)
	{
		if (context->after_syscall)
			return ft_dprintf(STDERR_FILENO, "%p", (void *)value);
		array_size = registers_get_param(context->regs, context->type, context->arg_index - 1);
	}
	int *local_array_ptr = malloc(array_size * sizeof(int));
	if (local_array_ptr == NULL)
	{
		log_error("log_INT_ARRAY", "malloc failed", true);
		return size_written;
	}
	if (remote_memcpy(local_array_ptr, context->pid, (void *)value, array_size * sizeof(int)) < 0)
	{
		size_written += ft_dprintf(STDERR_FILENO, "%p", (void *)value);
		free(local_array_ptr);
		return size_written;
	}
	size_written += ft_dprintf(STDERR_FILENO, "[");
	bool_t first = true;
	for (int i = 0; i < array_size; i++)
	{
		if (!first)
			size_written += ft_dprintf(STDERR_FILENO, ", ");
		first = false;
		size_written += ft_dprintf(STDERR_FILENO, "%d", local_array_ptr[i]);
	}
	size_written += ft_dprintf(STDERR_FILENO, "]");
	free(local_array_ptr);
	return size_written;
}