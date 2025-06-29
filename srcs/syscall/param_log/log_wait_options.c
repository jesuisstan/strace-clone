#include "param_log.h"
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

static const flag_str_t wait_flags[] = {
	FLAG_STR(WNOHANG), FLAG_STR(WUNTRACED), FLAG_STR(WCONTINUED),
	FLAG_STR(WEXITED), FLAG_STR(WSTOPPED),	FLAG_STR(WNOWAIT),
};

/**
 * @brief Log wait options
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_WAIT_OPTIONS(uint64_t value)
{
	return flags_log(value, wait_flags, sizeof(wait_flags) / sizeof(wait_flags[0]));
}