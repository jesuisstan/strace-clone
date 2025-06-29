#define _GNU_SOURCE

#include "param_log.h"
#include <sys/file.h>
#include <unistd.h>

static const flag_str_t flock_cmds[] = {
	FLAG_STR(LOCK_SH),	 FLAG_STR(LOCK_EX),	  FLAG_STR(LOCK_NB),	FLAG_STR(LOCK_UN),
	FLAG_STR(LOCK_MAND), FLAG_STR(LOCK_READ), FLAG_STR(LOCK_WRITE), FLAG_STR(LOCK_RW),
};

/**
 * @brief Log fcntl cmd
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_FLOCK_CMD(uint64_t value)
{
	return option_log(value, flock_cmds, sizeof(flock_cmds) / sizeof(flock_cmds[0]), "LOCK_???");
}