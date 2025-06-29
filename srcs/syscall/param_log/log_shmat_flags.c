#include "param_log.h"
#include <sys/shm.h>
#include <sys/types.h>

static const flag_str_t shmat_flags[] = {
	FLAG_STR(SHM_RDONLY),
	FLAG_STR(SHM_RND),
	FLAG_STR(SHM_REMAP),
	FLAG_STR(SHM_EXEC),
};

/**
 * @brief Log shmat flags
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_SHMAT_FLAGS(uint64_t value)
{
	return flags_log(value, shmat_flags, sizeof(shmat_flags) / sizeof(shmat_flags[0]));
}