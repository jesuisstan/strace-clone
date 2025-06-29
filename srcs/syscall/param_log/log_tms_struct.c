#define _GNU_SOURCE

#include "param_log.h"
#include <sys/sysinfo.h>
#include <sys/times.h>

/**
 * @brief Log tms struct
 *
 * @param value the ptr to tms struct
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_TMS_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct tms, tms);
	return dprintf(STDERR_FILENO,
					  "{tms_utime=%ld, tms_stime=%ld, tms_cutime=%ld, tms_cstime=%ld}",
					  tms.tms_utime, tms.tms_stime, tms.tms_cutime, tms.tms_cstime);
}