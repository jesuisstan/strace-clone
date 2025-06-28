#include "param_log.h"
#include <ft_printf.h>

int log_SIGNED_INT(int64_t value)
{
	return ft_dprintf(STDERR_FILENO, "%d", value);
}