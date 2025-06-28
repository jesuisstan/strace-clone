#include "param_log.h"
#include <ft_printf.h>
#include <sched.h>

int log_SCHED_PARAM_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct sched_param, sched_param);
	return ft_dprintf(STDERR_FILENO, "{sched_priority=%d}", sched_param.sched_priority);
}