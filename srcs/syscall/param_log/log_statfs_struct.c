#define _GNU_SOURCE
#include "param_log.h"
#include <stdio.h>
#include <sys/vfs.h>

int log_STATFS_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct statfs, statfs);
	return fprintf(stderr,
				  "{f_type=%#lx, f_bsize=%lu, f_blocks=%lu, f_bfree=%lu, f_bavail=%lu, "
				  "f_files=%lu, f_ffree=%lu, f_fsid={%d, %d}, f_namelen=%lu, f_frsize=%lu, "
				  "f_flags=%#lx}",
				  statfs.f_type, statfs.f_bsize, statfs.f_blocks, statfs.f_bfree,
				  statfs.f_bavail, statfs.f_files, statfs.f_ffree, statfs.f_fsid.__val[0],
				  statfs.f_fsid.__val[1], statfs.f_namelen, statfs.f_frsize, statfs.f_flags);
}