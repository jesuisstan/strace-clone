#include "param_log.h"
#include <macros.h>
#include <sys/mman.h>

static const flag_str_t mlockall_flags[] = {
	FLAG_STR(MCL_CURRENT),
	FLAG_STR(MCL_FUTURE),
};

int log_MLOCKALL_FLAGS(uint64_t value)
{
	return flags_log(value, mlockall_flags, ELEM_COUNT(mlockall_flags));
}