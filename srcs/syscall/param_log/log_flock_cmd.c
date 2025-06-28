#define _GNU_SOURCE

#include "param_log.h"
#include <fcntl.h>
#include <ft_printf.h>
#include <macros.h>
#include <unistd.h>

#define F_GETOWNER_UIDS 17

static const flag_str_t fcntl_cmds[] = {
	FLAG_STR(F_DUPFD),		FLAG_STR(F_GETFD),		   FLAG_STR(F_SETFD),	  FLAG_STR(F_GETFL),
	FLAG_STR(F_SETFL),		FLAG_STR(F_GETLK),		   FLAG_STR(F_SETLK),	  FLAG_STR(F_SETLKW),
	FLAG_STR(F_GETOWN),		FLAG_STR(F_SETOWN),		   FLAG_STR(F_GETSIG),	  FLAG_STR(F_SETSIG),
	FLAG_STR(F_GETLK64),	FLAG_STR(F_SETLK64),	   FLAG_STR(F_SETLKW64),  FLAG_STR(F_SETOWN_EX),
	FLAG_STR(F_GETOWN_EX),	FLAG_STR(F_GETOWNER_UIDS), FLAG_STR(F_OFD_GETLK), FLAG_STR(F_OFD_SETLK),
	FLAG_STR(F_OFD_SETLKW),
};

/**
 * @brief Log fcntl cmd
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_FCNTL_CMD(uint64_t value)
{
	return option_log(value, fcntl_cmds, ELEM_COUNT(fcntl_cmds), "F_???");
}