#include "param_log.h"
#include <ft_printf.h>

int log_NONE(void)
{
	return ft_dprintf(STDERR_FILENO, "?");
}