#include "param_log.h"
#include <fcntl.h>
#include <ft_printf.h>
#include <macros.h>

static const flag_str_t advice_flags[] = {
	FLAG_STR(POSIX_FADV_NORMAL),   FLAG_STR(POSIX_FADV_RANDOM),	  FLAG_STR(POSIX_FADV_SEQUENTIAL),
	FLAG_STR(POSIX_FADV_WILLNEED), FLAG_STR(POSIX_FADV_DONTNEED), FLAG_STR(POSIX_FADV_NOREUSE),
};

int log_ADVISE(uint64_t value)
{
	return option_log(value, advice_flags, ELEM_COUNT(advice_flags), "POSIX_FADV_???");
}