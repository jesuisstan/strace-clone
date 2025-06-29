#include "param_log.h"

/**
 * @brief Log a hex value
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_HEX(uint64_t value)
{
	if (value == 0)
		return dprintf(STDERR_FILENO, "0");
	return dprintf(STDERR_FILENO, "%#lx", value);
}