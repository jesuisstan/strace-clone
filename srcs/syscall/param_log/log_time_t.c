#include <time.h>

#include "param_log.h"
#include <ft_printf.h>

int log_TIME_T(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(time_t, tv);
	return ft_dprintf(STDERR_FILENO, "%ld", tv);
}