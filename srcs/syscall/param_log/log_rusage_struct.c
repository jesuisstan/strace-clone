#define _GNU_SOURCE

#include "param_log.h"
#include <ft_strace_utils.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>

/**
 * @brief Log a rusage struct
 *
 * @param value the ptr to rusage
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_RUSAGE_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct rusage, rusage);
	return dprintf(
		STDERR_FILENO,
		"{ru_utime={tv_sec=%ld, tv_usec=%ld}, ru_stime={tv_sec=%ld, tv_usec=%ld}, ...}",
		rusage.ru_utime.tv_sec, rusage.ru_utime.tv_usec, rusage.ru_stime.tv_sec,
		rusage.ru_stime.tv_usec);
}