#include "param_log.h"
#include <ft_printf.h>

/**
 * @brief log a hexadecimal value
 *
 * @param value
 * @return int
 */
int log_HEX(uint64_t value)
{
	if (value == 0)
		return ft_dprintf(STDERR_FILENO, "0");
	return ft_dprintf(STDERR_FILENO, "%#llx", value);
}