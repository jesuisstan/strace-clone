#include "param_log.h"
#include <errno.h>

int log_FD_NUMBER(uint64_t value)
{
	// Для file descriptor'ов просто показываем число, но можно добавить специальные случаи
	switch (value) {
	case 0:
		return dprintf(STDERR_FILENO, "STDIN_FILENO");
	case 1:
		return dprintf(STDERR_FILENO, "STDOUT_FILENO");
	case 2:
		return dprintf(STDERR_FILENO, "STDERR_FILENO");
	default:
		return dprintf(STDERR_FILENO, "%lu", value);
	}
}
