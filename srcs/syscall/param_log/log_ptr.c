#include "param_log.h"

int log_PTR(uint64_t value)
{
	if (value == 0)
		return dprintf(STDERR_FILENO, "NULL");
	return dprintf(STDERR_FILENO, "%p", (void *)value);
}