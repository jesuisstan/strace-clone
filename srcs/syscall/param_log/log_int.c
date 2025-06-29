#include "param_log.h"

/**
 * @brief Log an unsigned integer
 *
 * @param value value to log
 * @return int number of bytes written
 */
int log_INT(uint64_t value)
{
	return dprintf(STDERR_FILENO, "%lu", value);
}