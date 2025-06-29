#include "param_log.h"

int log_SIGNED_INT(int64_t value)
{
	return dprintf(STDERR_FILENO, "%ld", value);
}