#include "param_log.h"
#include <errno.h>

int log_PID_NUMBER(uint64_t value)
{
	// Для PID просто показываем число, но можно добавить специальные случаи
	switch (value) {
	case 0:
		return dprintf(STDERR_FILENO, "0");
	case -1:
		return dprintf(STDERR_FILENO, "-1");
	default:
		return dprintf(STDERR_FILENO, "%lu", value);
	}
}
