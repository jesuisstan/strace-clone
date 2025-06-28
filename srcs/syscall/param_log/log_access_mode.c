#include "param_log.h"
#include <ft_printf.h>
#include <macros.h>
#include <unistd.h>

static const flag_str_t flags[] = {
	FLAG_STR(R_OK),
	FLAG_STR(W_OK),
	FLAG_STR(X_OK),
};

/**
 * @brief Log access mode flags
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_ACCESS_MODE(uint64_t value)
{
	if (value == 0)
		return ft_dprintf(STDERR_FILENO, "F_OK");
	return flags_log(value, flags, ELEM_COUNT(flags));
}