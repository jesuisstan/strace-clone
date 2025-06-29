#define _GNU_SOURCE

#include "param_log.h"
#include <sys/msg.h>

static const flag_str_t flags[] = {
	FLAG_STR(IPC_NOWAIT),
	FLAG_STR(MSG_EXCEPT),
	FLAG_STR(MSG_NOERROR),
	FLAG_STR(MSG_COPY),
};

/**
 * @brief Log msgflg
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_MSGFLG(uint64_t value)
{
	return flags_log(value, flags, sizeof(flags) / sizeof(flags[0]));
}