#include "param_log.h"
#include <ft_printf.h>
#include <signal.h>

int log_SIGEVENT_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct sigevent, sigevent);
	return ft_dprintf(STDERR_FILENO, "{sigev_value=%d, sigev_signo=%d, sigev_notify=%d}",
					  sigevent.sigev_value.sival_int, sigevent.sigev_signo, sigevent.sigev_notify);
}