#include "param_log.h"

/**
 * @brief Log a signal name
 *
 * @param value
 * @return int
 */
int log_SIGNAL_NAME(uint64_t value)
{
	return dprintf(STDERR_FILENO, "%s", ft_signalname(value));
}