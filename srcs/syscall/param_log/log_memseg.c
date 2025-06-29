#define _GNU_SOURCE

#include "param_log.h"
#include <ft_strace_utils.h>
#include <sys/uio.h>
#include <stdint.h>

static int64_t REGISTERS_GET_RETURN(void *regs __attribute__((unused)), int type __attribute__((unused))) { return 0; }
static int64_t registers_get_param(void *regs __attribute__((unused)), int type __attribute__((unused)), int index __attribute__((unused))) { return 0; }

#define MAX_PRINT_SIZE 32
#define MAX_DISPLAY_SIZE 30  // Show only 30 chars, then ...

/**
 * @brief Log a memory segment from a remote process
 *
 * @param pid the pid of the remote process
 * @param remote_ptr the pointer to the memory segment in the remote process
 * @param buffer_size the size of the memory segment
 * @return int
 */
int log_memseg_remote(pid_t pid, void *remote_ptr, size_t buffer_size)
{
	size_t to_read = buffer_size > MAX_PRINT_SIZE ? MAX_PRINT_SIZE : buffer_size;
	char *buffer = malloc(to_read);
	if (!buffer)
	{
		log_error("log_MEM", "malloc failed", true);
		return 0;
	}
	if (remote_memcpy(buffer, pid, remote_ptr, to_read) < 0)
	{
		free(buffer);
		return dprintf(STDERR_FILENO, "%p", remote_ptr);
	}
	
	// For display, we want to show only a reasonable number of characters
	size_t display_size = to_read > MAX_DISPLAY_SIZE ? MAX_DISPLAY_SIZE : to_read;
	char *escaped_buffer = ft_escape(buffer, display_size);
	int size_written;
	if (buffer_size > display_size)
		size_written = dprintf(STDERR_FILENO, "\"%s\"...", escaped_buffer);
	else
		size_written = dprintf(STDERR_FILENO, "\"%s\"", escaped_buffer);
	free(escaped_buffer);
	free(buffer);
	return size_written;
}

/**
 * @brief log memory segment
 *
 * @param value the value
 * @param context the context
 */
int log_MEMSEG(uint64_t value, syscall_log_param_t *context)
{
	if (value == 0)
		return dprintf(STDERR_FILENO, "NULL");
	int64_t buffer_size = REGISTERS_GET_RETURN(context->regs, context->type);
	if (buffer_size < 0)
	{
		if (context->after_syscall)
			return dprintf(STDERR_FILENO, "%p", (void *)value);
		buffer_size = registers_get_param(context->regs, context->type, context->arg_index + 1);
	}
	return log_memseg_remote(context->pid, (void *)value, buffer_size);
}