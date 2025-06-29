#include "param_log.h"

int log_NONE(void)
{
	return dprintf(STDERR_FILENO, "?");
}