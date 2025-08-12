#define _GNU_SOURCE

#include "param_log.h"
#include <linux/random.h>

static const flag_str_t getrandom_flags[] = {
	FLAG_STR(GRND_NONBLOCK),
	FLAG_STR(GRND_RANDOM),
};

int log_GETRANDOM_FLAGS(uint64_t value)
{
	return flags_log(value, getrandom_flags, sizeof(getrandom_flags) / sizeof(getrandom_flags[0]));
}
