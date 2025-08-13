#include "param_log.h"
#include <sys/socket.h>
#include <errno.h>

int log_SEND_FLAGS(uint64_t value)
{
	bool first = true;
	int printed = 0;

	if (value == 0) {
		return dprintf(STDERR_FILENO, "0");
	}

	if (value & MSG_OOB) {
		printed += dprintf(STDERR_FILENO, "MSG_OOB");
		first = false;
	}

	if (value & MSG_PEEK) {
		if (!first) printed += dprintf(STDERR_FILENO, "|");
		printed += dprintf(STDERR_FILENO, "MSG_PEEK");
		first = false;
	}

	if (value & MSG_DONTROUTE) {
		if (!first) printed += dprintf(STDERR_FILENO, "|");
		printed += dprintf(STDERR_FILENO, "MSG_DONTROUTE");
		first = false;
	}

	if (value & MSG_CTRUNC) {
		if (!first) printed += dprintf(STDERR_FILENO, "|");
		printed += dprintf(STDERR_FILENO, "MSG_CTRUNC");
		first = false;
	}

	if (value & MSG_TRUNC) {
		if (!first) printed += dprintf(STDERR_FILENO, "|");
		printed += dprintf(STDERR_FILENO, "MSG_TRUNC");
		first = false;
	}

	if (value & MSG_DONTWAIT) {
		if (!first) printed += dprintf(STDERR_FILENO, "|");
		printed += dprintf(STDERR_FILENO, "MSG_DONTWAIT");
		first = false;
	}

	if (value & MSG_EOR) {
		if (!first) printed += dprintf(STDERR_FILENO, "|");
		printed += dprintf(STDERR_FILENO, "MSG_EOR");
		first = false;
	}

	if (value & MSG_WAITALL) {
		if (!first) printed += dprintf(STDERR_FILENO, "|");
		printed += dprintf(STDERR_FILENO, "MSG_WAITALL");
		first = false;
	}

	if (value & MSG_FIN) {
		if (!first) printed += dprintf(STDERR_FILENO, "|");
		printed += dprintf(STDERR_FILENO, "MSG_FIN");
		first = false;
	}

	if (value & MSG_SYN) {
		if (!first) printed += dprintf(STDERR_FILENO, "|");
		printed += dprintf(STDERR_FILENO, "MSG_SYN");
		first = false;
	}

	if (value & MSG_CONFIRM) {
		if (!first) printed += dprintf(STDERR_FILENO, "|");
		printed += dprintf(STDERR_FILENO, "MSG_CONFIRM");
		first = false;
	}

	if (value & MSG_RST) {
		if (!first) printed += dprintf(STDERR_FILENO, "|");
		printed += dprintf(STDERR_FILENO, "MSG_RST");
		first = false;
	}

	if (value & MSG_ERRQUEUE) {
		if (!first) printed += dprintf(STDERR_FILENO, "|");
		printed += dprintf(STDERR_FILENO, "MSG_ERRQUEUE");
		first = false;
	}

	if (value & MSG_NOSIGNAL) {
		if (!first) printed += dprintf(STDERR_FILENO, "|");
		printed += dprintf(STDERR_FILENO, "MSG_NOSIGNAL");
		first = false;
	}

	if (value & MSG_MORE) {
		if (!first) printed += dprintf(STDERR_FILENO, "|");
		printed += dprintf(STDERR_FILENO, "MSG_MORE");
		first = false;
	}

	if (value & MSG_WAITFORONE) {
		if (!first) printed += dprintf(STDERR_FILENO, "|");
		printed += dprintf(STDERR_FILENO, "MSG_WAITFORONE");
		first = false;
	}

	if (value & MSG_BATCH) {
		if (!first) printed += dprintf(STDERR_FILENO, "|");
		printed += dprintf(STDERR_FILENO, "MSG_BATCH");
		first = false;
	}

	if (value & MSG_ZEROCOPY) {
		if (!first) printed += dprintf(STDERR_FILENO, "|");
		printed += dprintf(STDERR_FILENO, "MSG_ZEROCOPY");
		first = false;
	}

	if (value & MSG_FASTOPEN) {
		if (!first) printed += dprintf(STDERR_FILENO, "|");
		printed += dprintf(STDERR_FILENO, "MSG_FASTOPEN");
		first = false;
	}

	if (value & MSG_CMSG_CLOEXEC) {
		if (!first) printed += dprintf(STDERR_FILENO, "|");
		printed += dprintf(STDERR_FILENO, "MSG_CMSG_CLOEXEC");
		first = false;
	}

	return printed;
}