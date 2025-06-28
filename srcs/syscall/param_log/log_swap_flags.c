#define _GNU_SOURCE

#include "param_log.h"
#include <macros.h>
#include <sys/swap.h>

static const flag_str_t swap_flags[] = {
	FLAG_STR(SWAP_FLAG_PREFER),
	FLAG_STR(SWAP_FLAG_PRIO_MASK),
	FLAG_STR(SWAP_FLAG_PRIO_SHIFT),
	FLAG_STR(SWAP_FLAG_DISCARD),
};

/**
 * @brief Log swap flags
 *
 * @param value the value to log
 */
int log_SWAP_FLAGS(uint64_t value)
{
	return flags_log(value, swap_flags, ELEM_COUNT(swap_flags));
}