#include "param_log.h"
#include <utime.h>

int log_UTIMBUF_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct utimbuf, utimbuf);
	return dprintf(STDERR_FILENO, "{actime=%ld, modtime=%ld}", utimbuf.actime, utimbuf.modtime);
}