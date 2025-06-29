#include "param_log.h"
#include <sys/timex.h>

int log_KERNEL_TIMEX_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct timex, timex);
	return dprintf(STDERR_FILENO,
					  "{modes=%#x, offset=%ld, freq=%ld, maxerror=%ld, esterror=%ld, "
					  "status=%#x, constant=%ld, precision=%ld, tolerance=%ld, "
					  "time={tv_sec=%ld, tv_usec=%ld}, tick=%ld, ppsfreq=%ld, jitter=%ld, shift=%d, "
					  "stabil=%ld, jitcnt=%ld, calcnt=%ld, errcnt=%ld, "
					  "stbcnt=%ld, tai=%d}",
					  timex.modes, timex.offset, timex.freq, timex.maxerror, timex.esterror,
					  timex.status, timex.constant, timex.precision, timex.tolerance, timex.time.tv_sec, timex.time.tv_usec,
					  timex.tick, timex.ppsfreq, timex.jitter, timex.shift, timex.stabil,
					  timex.jitcnt, timex.calcnt, timex.errcnt, timex.stbcnt, timex.tai);
}