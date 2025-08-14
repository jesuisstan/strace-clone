#include "param_log.h"

int log_SIGNED_INT(int64_t value)
{
	// Handle the case where value is actually a signed integer
	// that was passed as unsigned
	if (value == 4294967295ULL) {
		return dprintf(STDERR_FILENO, "-1");
	}
	return dprintf(STDERR_FILENO, "%ld", value);
}