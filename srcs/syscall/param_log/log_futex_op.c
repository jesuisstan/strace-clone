#include "param_log.h"
#include <linux/futex.h>
#include <errno.h>

int log_FUTEX_OP(uint64_t value)
{
	switch (value) {
	case FUTEX_WAIT:
		return dprintf(STDERR_FILENO, "FUTEX_WAIT");
	case FUTEX_WAKE:
		return dprintf(STDERR_FILENO, "FUTEX_WAKE");
	case FUTEX_FD:
		return dprintf(STDERR_FILENO, "FUTEX_FD");
	case FUTEX_REQUEUE:
		return dprintf(STDERR_FILENO, "FUTEX_REQUEUE");
	case FUTEX_CMP_REQUEUE:
		return dprintf(STDERR_FILENO, "FUTEX_CMP_REQUEUE");
	case FUTEX_WAKE_OP:
		return dprintf(STDERR_FILENO, "FUTEX_WAKE_OP");
	case FUTEX_LOCK_PI:
		return dprintf(STDERR_FILENO, "FUTEX_LOCK_PI");
	case FUTEX_UNLOCK_PI:
		return dprintf(STDERR_FILENO, "FUTEX_UNLOCK_PI");
	case FUTEX_TRYLOCK_PI:
		return dprintf(STDERR_FILENO, "FUTEX_TRYLOCK_PI");
	case FUTEX_WAIT_BITSET:
		return dprintf(STDERR_FILENO, "FUTEX_WAIT_BITSET");
	case FUTEX_WAKE_BITSET:
		return dprintf(STDERR_FILENO, "FUTEX_WAKE_BITSET");
	case FUTEX_WAIT_REQUEUE_PI:
		return dprintf(STDERR_FILENO, "FUTEX_WAIT_REQUEUE_PI");
	case FUTEX_CMP_REQUEUE_PI:
		return dprintf(STDERR_FILENO, "FUTEX_CMP_REQUEUE_PI");
	default:
		return dprintf(STDERR_FILENO, "%lu", value);
	}
}
