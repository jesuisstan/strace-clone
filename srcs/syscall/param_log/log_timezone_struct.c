#define _GNU_SOURCE

#include "param_log.h"
#include <ft_printf.h>
#include <sys/time.h>

/**
 * @brief Log timezone struct
 *
 * @param value the ptr to timezone struct
 * @return int the number of bytes written
 */
int log_TIMEZONE_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct timezone, tz);
	return ft_dprintf(STDERR_FILENO, "{tz_minuteswest=%d, tz_dsttime=%d}", tz.tz_minuteswest,
					  tz.tz_dsttime);
}