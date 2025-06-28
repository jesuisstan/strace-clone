#include "param_log.h"
#include <ft_printf.h>

int log_PTR(uint64_t value)
{
	if (value == 0)
		return ft_dprintf(STDERR_FILENO, "NULL");
	return ft_dprintf(STDERR_FILENO, "%p", (void *)value);
}