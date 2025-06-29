#include "param_log.h"
#include <sys/types.h>
#include <sys/wait.h>

static const flag_str_t waitid_which[] = {
	FLAG_STR(P_PID),
	FLAG_STR(P_PGID),
	FLAG_STR(P_ALL),
};

int log_WAITID_WHICH(uint64_t value)
{
	return option_log(value, waitid_which, sizeof(waitid_which) / sizeof(waitid_which[0]), "P_???");
}