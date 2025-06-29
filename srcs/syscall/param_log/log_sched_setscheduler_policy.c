#define _GNU_SOURCE

#include "param_log.h"
#include <sched.h>

static const flag_str_t sched_policy_flags[] = {
	FLAG_STR(SCHED_OTHER), FLAG_STR(SCHED_FIFO), FLAG_STR(SCHED_RR),
	FLAG_STR(SCHED_BATCH), FLAG_STR(SCHED_IDLE), FLAG_STR(SCHED_DEADLINE),
};

int log_SCHED_SETCHEDULER_POLICY(uint64_t value)
{
	return option_log(value, sched_policy_flags, sizeof(sched_policy_flags) / sizeof(sched_policy_flags[0]), "SCHED_???");
}