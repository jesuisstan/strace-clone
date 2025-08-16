#define _GNU_SOURCE

#include "param_log.h"
#include <ft_strace_utils.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <stdbool.h>
#include <sys/sysmacros.h>

static const flag_str_t flags[] = {
	FLAG_STR(S_IFREG),
	FLAG_STR(S_IFDIR),
	FLAG_STR(S_IFCHR),
	FLAG_STR(S_IFBLK),
	FLAG_STR(S_IFIFO),
	FLAG_STR(S_IFLNK),
	FLAG_STR(S_IFSOCK),
};
/**
 * @brief Log stat struct
 *
 * @param value
 * @param context
 * @return int
 */
int log_STAT_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct stat, stat);
	int size_written = 0;
	size_written += dprintf(STDERR_FILENO, "{st_mode=");
	bool first = true;
	for (size_t i = 0; i < sizeof(flags) / sizeof(flags[0]); i++)
	{
		if (stat.st_mode & flags[i].flag)
		{
			if (!first)
				size_written += dprintf(STDERR_FILENO, "|");
			size_written += dprintf(STDERR_FILENO, "%s", flags[i].str);
			first = false;
			stat.st_mode &= ~flags[i].flag;
		}
	}
	if (stat.st_mode)
	{
		if (!first)
			size_written += dprintf(STDERR_FILENO, "|");
		size_written += dprintf(STDERR_FILENO, "%#o", stat.st_mode);
	}
	size_written += dprintf(STDERR_FILENO, ", st_size=%ld", stat.st_size);
	
	// Add st_rdev for character and block devices
	if (S_ISCHR(stat.st_mode) || S_ISBLK(stat.st_mode)) {
		size_written += dprintf(STDERR_FILENO, ", st_rdev=makedev(0x%lx, 0x%lx)", 
			(unsigned long)major(stat.st_rdev), (unsigned long)minor(stat.st_rdev));
	}
	
	size_written += dprintf(STDERR_FILENO, ", ...}");
	return size_written;
}