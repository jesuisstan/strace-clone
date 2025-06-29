#define _GNU_SOURCE

#include "param_log.h"
#include <sys/resource.h>
#include <sys/time.h>

static const flag_str_t rlimit_max[] = {
	FLAG_STR(RLIM64_INFINITY),
};

static int log_rlimit_cur(uint64_t value)
{
	uint64_t n_block = value / 1024;
	uint64_t n_byte = value % 1024;
	if (n_block == 0 || n_byte != 0)
		return dprintf(STDERR_FILENO, "%lu", value);
	return dprintf(STDERR_FILENO, "%lu*1024", n_block);
}

/**
 * @brief Log rlimit struct
 *
 * @param value the value to log
 * @param context the context of the syscall
 *@return int the number of bytes written
 */
int log_RLIMIT_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct rlimit, rlimit);
	int size_written = 0;
	size_written += dprintf(STDERR_FILENO, "{rlim_cur=");
	size_written += log_rlimit_cur(rlimit.rlim_cur);
	size_written += dprintf(STDERR_FILENO, ", rlim_max=");
	size_written += option_log(rlimit.rlim_max, rlimit_max, sizeof(rlimit_max) / sizeof(rlimit_max[0]), NULL);
	size_written += dprintf(STDERR_FILENO, "}");
	return size_written;
}