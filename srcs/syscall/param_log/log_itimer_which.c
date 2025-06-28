#include "param_log.h"
#include <ft_printf.h>
#include <macros.h>
#include <sys/time.h>

static const flag_str_t flags[] = {
	FLAG_STR(ITIMER_REAL),
	FLAG_STR(ITIMER_VIRTUAL),
	FLAG_STR(ITIMER_PROF),
};

/**
 * @brief Log setitimer which
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_ITIMER_WHICH(uint64_t value)
{
	return option_log(value, flags, ELEM_COUNT(flags), "ITIMER_???");
}