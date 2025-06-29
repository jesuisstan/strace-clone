#include "param_log.h"
#include <fcntl.h>

int log_OPENAT_DIRFD(int value)
{
	if (value == AT_FDCWD)
		return dprintf(STDERR_FILENO, "AT_FDCWD");
	return dprintf(STDERR_FILENO, "%d", value);
}