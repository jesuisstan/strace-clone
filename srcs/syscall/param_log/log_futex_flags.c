#include "param_log.h"
#include <linux/futex.h>
#include <errno.h>

int log_FUTEX_FLAGS(uint64_t value)
{
	bool first = true;
	int printed = 0;

	if (value == 0) {
		return dprintf(STDERR_FILENO, "0");
	}

	if (value & FUTEX_PRIVATE_FLAG) {
		printed += dprintf(STDERR_FILENO, "FUTEX_PRIVATE_FLAG");
		first = false;
	}

	if (value & FUTEX_CLOCK_REALTIME) {
		if (!first) printed += dprintf(STDERR_FILENO, "|");
		printed += dprintf(STDERR_FILENO, "FUTEX_CLOCK_REALTIME");
		first = false;
	}

	if (value & FUTEX_CMD_MASK) {
		// Extract the command part
		uint64_t cmd = value & FUTEX_CMD_MASK;
		if (!first) printed += dprintf(STDERR_FILENO, "|");
		printed += dprintf(STDERR_FILENO, "0x%lx", cmd);
	}

	return printed;
}
