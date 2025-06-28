#define _GNU_SOURCE
#include "param_log.h"
#include <ft_printf.h>
#include <sys/vfs.h>

int log_STATFS_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct statfs, statfs);
	return ft_dprintf(STDERR_FILENO,
					  "{f_type=%#x, f_bsize=%u, f_blocks=%lu, f_bfree=%lu, f_bavail=%lu, "
					  "f_files=%lu, f_ffree=%lu, f_fsid={%d, %d}, f_namelen=%u, f_frsize=%u, "
					  "f_flags=%#x}",
					  statfs.f_type, statfs.f_bsize, statfs.f_blocks, statfs.f_bfree,
					  statfs.f_bavail, statfs.f_files, statfs.f_ffree, statfs.f_fsid.__val[0],
					  statfs.f_fsid.__val[1], statfs.f_namelen, statfs.f_frsize, statfs.f_flags);
}