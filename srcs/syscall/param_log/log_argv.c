#define _GNU_SOURCE

#include "param_log.h"
#include <ft_strace_utils.h>
#include <sys/select.h>
#include <sys/uio.h>
#include <stdbool.h>

#define MAX_STR_LEN 32 + 1

/**
 * @brief Log argv
 *
 * @param value argv
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_ARGV(uint64_t value, syscall_log_param_t *context)
{
	if (value == 0)
		return dprintf(STDERR_FILENO, "NULL");
	char **str_tab = (char **)value;
	int size_written = 0;
	size_written += dprintf(STDERR_FILENO, "[");
	bool first = true;
	while (true)
	{
		char *addr_remote_str = NULL;
		if (remote_memcpy(&addr_remote_str, context->pid, str_tab, sizeof(char *)) < 0)
			break;
		if (addr_remote_str == NULL)
			break;
		if (!first)
			size_written += dprintf(STDERR_FILENO, ", ");
		first = false;
		size_written += log_remote_string(context->pid, addr_remote_str, MAX_STR_LEN);
		str_tab++;
	}
	size_written += dprintf(STDERR_FILENO, "]");
	return size_written;
}