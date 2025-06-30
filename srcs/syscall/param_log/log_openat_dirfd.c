#include "param_log.h"
#include <fcntl.h>

int log_OPENAT_DIRFD(int value)
{
	if ((int)value == AT_FDCWD)
		return dprintf(STDERR_FILENO, "AT_FDCWD");
	return dprintf(STDERR_FILENO, "%d", (int)value);
}