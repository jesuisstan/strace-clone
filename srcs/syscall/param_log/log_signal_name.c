#include "param_log.h"
#include <ft_printf.h>
#include <ft_string.h>

/**
 * @brief Log a signal name
 *
 * @param value
 * @return int
 */
int log_SIGNAL_NAME(uint64_t value)
{
	return ft_dprintf(STDERR_FILENO, "%s", ft_signalname(value));
}