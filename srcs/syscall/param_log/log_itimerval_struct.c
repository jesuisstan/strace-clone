#define _GNU_SOURCE

#include "param_log.h"
#include <ft_printf.h>
#include <ft_strace_utils.h>
#include <macros.h>
#include <registers.h>
#include <sys/time.h>
#include <sys/uio.h>

/**
 * @brief Log a itimerval struct
 *
 * @param value the ptr to itimerval
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_ITIMERVAL_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct itimerval, itimerval);
	return ft_dprintf(STDERR_FILENO,
					  "{it_interval={tv_sec=%ld, tv_usec=%ld}, it_value={tv_sec=%ld, tv_usec=%ld}}",
					  itimerval.it_interval.tv_sec, itimerval.it_interval.tv_usec,
					  itimerval.it_value.tv_sec, itimerval.it_value.tv_usec);
}