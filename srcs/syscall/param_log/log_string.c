#define _GNU_SOURCE

#include "param_log.h"
#include <ft_printf.h>
#include <ft_strace_utils.h>
#include <ft_string.h>
#include <registers.h>
#include <sys/uio.h>

#define DEFAULT_BUFFER_SIZE 32

typedef struct
{
	char *buffer;
	size_t size_buffer;
	size_t index;
} buffer_t;

static void buffer_add_char(buffer_t *buffer, char c)
{
	if (buffer->index >= buffer->size_buffer)
	{
		buffer->size_buffer *= 2;
		buffer->buffer = realloc(buffer->buffer, buffer->size_buffer);
		if (buffer->buffer == NULL)
		{
			log_error("log_STRING", "realloc failed", true);
			return;
		}
	}
	buffer->buffer[buffer->index++] = c;
}

/**
 * @brief Log a remote string
 *
 * @param pid the pid of the remote process
 * @param remote_str the remote string
 * @param max_size the max size of the string
 * @return int the number of bytes written
 */
int log_remote_string(pid_t pid, char *remote_str, int max_size)
{
	if (remote_str == NULL)
		return ft_dprintf(STDERR_FILENO, "NULL");
	buffer_t buffer = {
		.buffer = malloc(DEFAULT_BUFFER_SIZE),
		.size_buffer = DEFAULT_BUFFER_SIZE,
		.index = 0,
	};
	if (buffer.buffer == NULL)
	{
		log_error("log_STRING", "malloc failed", true);
		return 0;
	}
	char c = 1; // dummy value that will be overwritten by the first read
	while (c != '\0')
	{
		struct iovec local = {
			.iov_base = &c,
			.iov_len = 1,
		};
		struct iovec remote = {
			.iov_base = (void *)remote_str + buffer.index,
			.iov_len = 1,
		};
		if (process_vm_readv(pid, &local, 1, &remote, 1, 0) < 0)
		{
			log_error("log_STRING", "process_vm_readv failed", true);
			free(buffer.buffer);
			return 0;
		}
		buffer_add_char(&buffer, c);
		if (max_size != NO_SIZE && buffer.index >= (uint)max_size)
			break;
	}
	char *escaped_buffer = ft_escape(buffer.buffer, buffer.index - 1);
	int written_bytes = 0;
	if (max_size != NO_SIZE && buffer.index >= (uint)max_size)
		written_bytes += ft_dprintf(STDERR_FILENO, "\"%s\"...", escaped_buffer);
	else
		written_bytes += ft_dprintf(STDERR_FILENO, "\"%s\"", escaped_buffer);
	free(escaped_buffer);
	free(buffer.buffer);
	return written_bytes;
}

/**
 * @brief log memory segment
 *
 * @param value the value
 * @param context the context
 */
int log_STRING(uint64_t value, syscall_log_param_t *context)
{
	return log_remote_string(context->pid, (char *)value, NO_SIZE);
}