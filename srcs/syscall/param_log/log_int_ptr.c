#define _GNU_SOURCE

#include "param_log.h"
#include <ft_printf.h>
#include <ft_strace_utils.h>
#include <macros.h>
#include <registers.h>
#include <sys/uio.h>

/**
 * @brief Log int ptr
 *
 * @param value the ptr to int
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_INT_PTR(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(int, local_value);
	return ft_dprintf(STDERR_FILENO, "[%d]", local_value);
}