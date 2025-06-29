#include "param_log.h"
#include <ft_strace_utils.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdbool.h>

// Stub functions for missing register access
static int64_t REGISTERS_GET_RETURN(void *regs __attribute__((unused)), int type __attribute__((unused))) { return 0; }
static int64_t registers_get_param(void *regs __attribute__((unused)), int type __attribute__((unused)), int index __attribute__((unused))) { return 0; }

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
		return dprintf(STDERR_FILENO, "NULL");
	int64_t array_size = REGISTERS_GET_RETURN(context->regs, context->type);
	if (array_size < 0)
	{
		if (context->after_syscall)
			return dprintf(STDERR_FILENO, "%p", (void *)value);
		array_size = registers_get_param(context->regs, context->type, context->arg_index - 1);
	}
	int *local_array_ptr = malloc(array_size * sizeof(int));
	if (local_array_ptr == NULL)
	{
		fprintf(stderr, "log_INT_ARRAY: malloc failed\n");
		return size_written;
	}
	if (remote_memcpy(local_array_ptr, context->pid, (void *)value, array_size * sizeof(int)) < 0)
	{
		size_written += dprintf(STDERR_FILENO, "%p", (void *)value);
		free(local_array_ptr);
		return size_written;
	}
	size_written += dprintf(STDERR_FILENO, "[");
	bool first = true;
	for (int i = 0; i < array_size; i++)
	{
		if (!first)
			size_written += dprintf(STDERR_FILENO, ", ");
		first = false;
		size_written += dprintf(STDERR_FILENO, "%d", local_array_ptr[i]);
	}
	size_written += dprintf(STDERR_FILENO, "]");
	free(local_array_ptr);
	return size_written;
}