#include "param_log.h"
#include <ft_printf.h>

struct kernel_timespec
{
	long tv_sec;
	long tv_nsec;
};

struct kernel_itimerspec
{
	struct kernel_timespec it_interval;
	struct kernel_timespec it_value;
};

int log_KERNEL_ITIMERSPEC_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct kernel_itimerspec, itimerspec);
	return ft_dprintf(
		STDERR_FILENO,
		"{it_interval={tv_sec=%lld, tv_nsec=%lld}, it_value={tv_sec=%lld, tv_nsec=%lld}}",
		itimerspec.it_interval.tv_sec, itimerspec.it_interval.tv_nsec, itimerspec.it_value.tv_sec,
		itimerspec.it_value.tv_nsec);
}