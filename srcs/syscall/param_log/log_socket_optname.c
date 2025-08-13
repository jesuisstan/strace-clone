#include "param_log.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>

int log_SOCKET_OPTNAME(uint64_t value)
{
	switch (value) {
	case SO_REUSEADDR:
		return dprintf(STDERR_FILENO, "SO_REUSEADDR");
	case SO_REUSEPORT:
		return dprintf(STDERR_FILENO, "SO_REUSEPORT");
	case SO_KEEPALIVE:
		return dprintf(STDERR_FILENO, "SO_KEEPALIVE");
	case SO_BROADCAST:
		return dprintf(STDERR_FILENO, "SO_BROADCAST");
	case SO_LINGER:
		return dprintf(STDERR_FILENO, "SO_LINGER");
	case SO_OOBINLINE:
		return dprintf(STDERR_FILENO, "SO_OOBINLINE");
	case SO_SNDBUF:
		return dprintf(STDERR_FILENO, "SO_SNDBUF");
	case SO_RCVBUF:
		return dprintf(STDERR_FILENO, "SO_RCVBUF");
	case SO_ERROR:
		return dprintf(STDERR_FILENO, "SO_ERROR");
	case SO_TYPE:
		return dprintf(STDERR_FILENO, "SO_TYPE");
	case TCP_NODELAY:
		return dprintf(STDERR_FILENO, "TCP_NODELAY");
	case TCP_INFO:
		return dprintf(STDERR_FILENO, "TCP_INFO");
	case TCP_QUICKACK:
		return dprintf(STDERR_FILENO, "TCP_QUICKACK");
	default:
		return dprintf(STDERR_FILENO, "%lu", value);
	}
}
