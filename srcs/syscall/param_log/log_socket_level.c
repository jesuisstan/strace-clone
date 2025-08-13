#include "param_log.h"
#include <sys/socket.h>
#include <errno.h>

int log_SOCKET_LEVEL(uint64_t value)
{
	switch (value) {
	case SOL_SOCKET:
		return dprintf(STDERR_FILENO, "SOL_SOCKET");
	case SOL_RAW:
		return dprintf(STDERR_FILENO, "SOL_RAW");
	case SOL_PACKET:
		return dprintf(STDERR_FILENO, "SOL_PACKET");
	default:
		return dprintf(STDERR_FILENO, "%lu", value);
	}
}
