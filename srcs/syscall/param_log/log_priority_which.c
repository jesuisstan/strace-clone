#include "param_log.h"
#include <sys/resource.h>

static const flag_str_t priority_flags[] = {
	FLAG_STR(PRIO_PROCESS),
	FLAG_STR(PRIO_PGRP),
	FLAG_STR(PRIO_USER),
};

int log_PRIORITY_WHICH(uint64_t uvalue)
{
	return option_log(uvalue, priority_flags, sizeof(priority_flags) / sizeof(priority_flags[0]), "PRIO_???");
}