#define _GNU_SOURCE

#include "param_log.h"
#include <ft_strace_utils.h>
#include <sys/select.h>
#include <sys/uio.h>

/**
 * @brief log a timeval struct
 *
 * @param value the ptr to timeval struct
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_TIMEVAL_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct timeval, tv);
	if (context->is_return_log && tv.tv_sec == 0 && tv.tv_usec == 0)
		return dprintf(STDERR_FILENO, "(Timeout)");
	return dprintf(STDERR_FILENO, "{tv_sec=%ld, tv_usec=%ld}", tv.tv_sec, tv.tv_usec);
}