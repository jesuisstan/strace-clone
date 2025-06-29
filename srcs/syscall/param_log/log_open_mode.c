#include "param_log.h"

/**
 * @brief Log open mode
 *
 * @param value the value to log
 */
int log_OPEN_MODE(uint64_t value)
{
	return printf("%#o", (unsigned int)value);
}