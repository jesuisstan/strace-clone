#define _GNU_SOURCE

#include "param_log.h"
#include <sys/mman.h>

static const flag_str_t msync_flags[] = {
	FLAG_STR(MS_ASYNC),
	FLAG_STR(MS_INVALIDATE),
	FLAG_STR(MS_SYNC),
};

/**
 * @brief Log msync flags
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_MSYNC_FLAGS(uint64_t value)
{
	return flags_log(value, msync_flags, sizeof(msync_flags) / sizeof(msync_flags[0]));
}