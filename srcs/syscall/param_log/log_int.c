#include "param_log.h"
#include <ft_printf.h>

/**
 * @brief Log an unsigned integer
 *
 * @param value value to log
 * @return int number of bytes written
 */
int log_INT(uint64_t value)
{
	return ft_dprintf(STDERR_FILENO, "%llu", value);
}