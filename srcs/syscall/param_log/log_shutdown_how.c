#include "param_log.h"
#include <sys/socket.h>

static const flag_str_t shutdown_how_flags[] = {
	FLAG_STR(SHUT_RD),
	FLAG_STR(SHUT_WR),
	FLAG_STR(SHUT_RDWR),
};

/**
 * @brief Log shutdown how flags
 *
 * @param value the value to log
 */
int log_SHUTDOWN_HOW(uint64_t value)
{
	return option_log(value, shutdown_how_flags, sizeof(shutdown_how_flags) / sizeof(shutdown_how_flags[0]), "SHUT_???");
}
