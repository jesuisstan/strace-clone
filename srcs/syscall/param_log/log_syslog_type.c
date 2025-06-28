#define _GNU_SOURCE

#include "param_log.h"
#include <ft_printf.h>
#include <macros.h>
#include <unistd.h>

#define SYSLOG_ACTION_CLOSE 0
#define SYSLOG_ACTION_OPEN 1
#define SYSLOG_ACTION_READ 2
#define SYSLOG_ACTION_READ_ALL 3
#define SYSLOG_ACTION_READ_CLEAR 4
#define SYSLOG_ACTION_CLEAR 5
#define SYSLOG_ACTION_CONSOLE_OFF 6
#define SYSLOG_ACTION_CONSOLE_ON 7
#define SYSLOG_ACTION_CONSOLE_LEVEL 8
#define SYSLOG_ACTION_SIZE_UNREAD 9
#define SYSLOG_ACTION_SIZE_BUFFER 10

static const flag_str_t syslog_types[] = {
	FLAG_STR(SYSLOG_ACTION_CLOSE),		   FLAG_STR(SYSLOG_ACTION_OPEN),
	FLAG_STR(SYSLOG_ACTION_READ),		   FLAG_STR(SYSLOG_ACTION_READ_ALL),
	FLAG_STR(SYSLOG_ACTION_READ_CLEAR),	   FLAG_STR(SYSLOG_ACTION_CLEAR),
	FLAG_STR(SYSLOG_ACTION_CONSOLE_OFF),   FLAG_STR(SYSLOG_ACTION_CONSOLE_ON),
	FLAG_STR(SYSLOG_ACTION_CONSOLE_LEVEL), FLAG_STR(SYSLOG_ACTION_SIZE_UNREAD),
	FLAG_STR(SYSLOG_ACTION_SIZE_BUFFER),
};

/**
 * @brief Log syslog types
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_SYSLOG_TYPE(uint64_t value)
{
	return flags_log(value, syslog_types, ELEM_COUNT(syslog_types));
}