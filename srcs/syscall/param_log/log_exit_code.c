#include "param_log.h"
#include <errno.h>

int log_EXIT_CODE(uint64_t value)
{
	// Для exit кодов просто показываем число, но можно добавить специальные случаи
	switch (value) {
	case 0:
		return dprintf(STDERR_FILENO, "0");
	case 1:
		return dprintf(STDERR_FILENO, "1");
	case 2:
		return dprintf(STDERR_FILENO, "2");
	case 126:
		return dprintf(STDERR_FILENO, "126");
	case 127:
		return dprintf(STDERR_FILENO, "127");
	case 128:
		return dprintf(STDERR_FILENO, "128");
	default:
		return dprintf(STDERR_FILENO, "%lu", value);
	}
}
