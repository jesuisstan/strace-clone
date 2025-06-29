#include "param_log.h"
#include <time.h>

static const flag_str_t timer_settime_flags[] = {
	FLAG_STR(TIMER_ABSTIME),
};

int log_TIMER_SETTIME_FLAGS(uint64_t value)
{
	return flags_log(value, timer_settime_flags, sizeof(timer_settime_flags) / sizeof(timer_settime_flags[0]));
}