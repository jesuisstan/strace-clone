#include "param_log.h"
#include <linux/ioprio.h>

static const flag_str_t ioprio_which[] = {
	FLAG_STR(IOPRIO_WHO_PROCESS),
	FLAG_STR(IOPRIO_WHO_PGRP),
	FLAG_STR(IOPRIO_WHO_USER),
};

int log_IOPRIO_WHICH(uint64_t value)
{
	return flags_log(value, ioprio_which, sizeof(ioprio_which) / sizeof(ioprio_which[0]));
}