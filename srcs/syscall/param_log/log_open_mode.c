#include "param_log.h"
#include <ft_printf.h>

/**
 * @brief Log open mode
 *
 * @param value the value to log
 */
int log_OPEN_MODE(uint64_t value)
{
	return ft_dprintf(STDERR_FILENO, "%#o", value);
}