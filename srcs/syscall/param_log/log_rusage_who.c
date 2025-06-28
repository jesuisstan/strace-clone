#define _GNU_SOURCE

#include "param_log.h"
#include <macros.h>
#include <sys/resource.h>
#include <sys/time.h>

static const flag_str_t rusage_who[] = {
	FLAG_STR(RUSAGE_SELF),
	FLAG_STR(RUSAGE_CHILDREN),
	FLAG_STR(RUSAGE_THREAD),
};

/**
 * @brief Log rusage who
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_RUSAGE_WHO(uint64_t value)
{
	return option_log(value, rusage_who, ELEM_COUNT(rusage_who), "RUSAGE_???");
}