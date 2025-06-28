#define _GNU_SOURCE

#include "param_log.h"
#include <ft_printf.h>
#include <ft_strace_utils.h>
#include <macros.h>
#include <registers.h>
#include <sys/uio.h>

typedef struct kernel_timespec
{
	long tv_sec;
	long tv_nsec;
} kernel_timespec_t;

/**
 * @brief Log a timespec struct
 *
 * @param value the ptr to timespec
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_KERNEL_TIMESPEC_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(kernel_timespec_t, timespec);
	return ft_dprintf(STDERR_FILENO, "{tv_sec=%ld, tv_nsec=%ld}", timespec.tv_sec,
					  timespec.tv_nsec);
}