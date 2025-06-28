#include "param_log.h"
#include <ft_printf.h>
#include <macros.h>
#include <signal.h>

static const flag_str_t flags[] = {
	FLAG_STR(SIG_BLOCK),
	FLAG_STR(SIG_UNBLOCK),
	FLAG_STR(SIG_SETMASK),
};

/**
 * @brief Log sigprocmask how
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_SIGPROCMASK_HOW(uint64_t value)
{
	return option_log(value, flags, ELEM_COUNT(flags), "SIG_???");
}
