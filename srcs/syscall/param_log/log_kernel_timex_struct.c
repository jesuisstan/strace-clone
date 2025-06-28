#include "param_log.h"
#include <ft_printf.h>
#include <sys/timex.h>

int log_KERNEL_TIMEX_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct timex, timex);
	return ft_dprintf(STDERR_FILENO,
					  "{modes=%#x, offset=%ld, freq=%ld, maxerror=%ld, esterror=%ld, "
					  "status=%#x, constant=%ld, precision=%ld, tolerance=%ld, "
					  "time=%ld, tick=%ld, ppsfreq=%ld, jitter=%ld, shift=%ld, "
					  "stabil=%ld, jitcnt=%ld, calcnt=%ld, errcnt=%ld, "
					  "stbcnt=%ld, tai=%ld}",
					  timex.modes, timex.offset, timex.freq, timex.maxerror, timex.esterror,
					  timex.status, timex.constant, timex.precision, timex.tolerance, timex.time,
					  timex.tick, timex.ppsfreq, timex.jitter, timex.shift, timex.stabil,
					  timex.jitcnt, timex.calcnt, timex.errcnt, timex.stbcnt, timex.tai);
}