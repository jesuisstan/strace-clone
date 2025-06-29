#define _GNU_SOURCE

#include "param_log.h"
#include <sys/sysinfo.h>

/**
 * @brief Log sysinfo struct
 *
 * @param value the ptr to sysinfo struct
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_SYSINFO_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct sysinfo, sysinfo);
	return dprintf(STDERR_FILENO,
					  "{uptime=%ld, loads=[%lu, %lu, %lu], totalram=%lu, freeram=%lu, "
					  "sharedram=%lu, bufferram=%lu, totalswap=%lu, freeswap=%lu, procs=%hu, "
					  "totalhigh=%lu, freehigh=%lu, mem_unit=%hu}",
					  sysinfo.uptime, sysinfo.loads[0], sysinfo.loads[1], sysinfo.loads[2],
					  sysinfo.totalram, sysinfo.freeram, sysinfo.sharedram, sysinfo.bufferram,
					  sysinfo.totalswap, sysinfo.freeswap, sysinfo.procs, sysinfo.totalhigh,
					  sysinfo.freehigh, sysinfo.mem_unit);
}
