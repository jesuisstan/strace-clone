#define _GNU_SOURCE

#include "param_log.h"
#include <ft_strace_utils.h>
#include <sys/uio.h>
#include <sys/utsname.h>

/**
 * @brief Log a utsname struct
 *
 * @param value the ptr to utsname struct
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_UTSNAME_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct utsname, utsname);
	return dprintf(STDERR_FILENO, "{sysname=\"%s\", nodename=\"%s\", ...}", utsname.sysname,
					  utsname.nodename);
}