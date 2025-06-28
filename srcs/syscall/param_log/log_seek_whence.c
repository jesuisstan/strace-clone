#include "param_log.h"
#include <ft_printf.h>
#include <macros.h>

static const flag_str_t flags[] = {
	FLAG_STR(SEEK_SET),
	FLAG_STR(SEEK_CUR),
	FLAG_STR(SEEK_END),
};

/**
 * @brief Log whence
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_SEEK_WHENCE(uint64_t value)
{
	return option_log(value, flags, ELEM_COUNT(flags), "SEEK_???");
}