#include "param_log.h"
#include <errno.h>

int log_PID_NUMBER(uint64_t value)
{
	// Handle special cases for PID values
	if (value == 0) {
		return dprintf(STDERR_FILENO, "0");
	} else if (value == 4294967295ULL) {
		// This is -1 represented as unsigned 32-bit value
		return dprintf(STDERR_FILENO, "-1");
	} else {
		return dprintf(STDERR_FILENO, "%lu", value);
	}
}
