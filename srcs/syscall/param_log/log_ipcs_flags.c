#include "param_log.h"
#include <sys/ipc.h>
#include <sys/sem.h>

static const flag_str_t ipcs_flags[] = {
	FLAG_STR(IPC_CREAT),
	FLAG_STR(IPC_EXCL),
	FLAG_STR(IPC_NOWAIT),
};

/**
 * @brief Log ipcs flags
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_IPCS_FLAGS(uint64_t value)
{
	return flags_log(value, ipcs_flags, sizeof(ipcs_flags) / sizeof(ipcs_flags[0]));
}