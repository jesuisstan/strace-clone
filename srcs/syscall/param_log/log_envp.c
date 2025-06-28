#define _GNU_SOURCE

#include "param_log.h"
#include <ft_printf.h>
#include <ft_strace_utils.h>
#include <macros.h>
#include <registers.h>
#include <sys/select.h>
#include <sys/uio.h>

/**
 * @brief Log envp struct
 *
 * @param value the ptr to envp
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_ENVP(uint64_t value, syscall_log_param_t *context)
{
	if (value == 0)
		return ft_dprintf(STDERR_FILENO, "NULL");
	char **str_tab = (char **)value;
	size_t var_count = 0;
	while (true)
	{
		char *addr_remote_str = NULL;
		if (remote_memcpy(&addr_remote_str, context->pid, str_tab, sizeof(char *)) < 0)
			break;
		if (addr_remote_str == NULL)
			break;
		var_count++;
		str_tab++;
	}
	return ft_dprintf(STDERR_FILENO, "%p /* %zu vars */", (void *)value, var_count);
}