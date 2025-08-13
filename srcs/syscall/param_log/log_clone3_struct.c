#include "param_log.h"
#include <sys/syscall.h>
#include <linux/sched.h>
#include <errno.h>

/**
 * @brief Log clone3 struct
 *
 * @param value the pointer to the clone3 struct
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_CLONE3_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	(void)context;
	if (value == 0) {
		return dprintf(STDERR_FILENO, "NULL");
	}
	
	// For now, just print the pointer value
	// In a full implementation, we would read and decode the clone3 struct
	return dprintf(STDERR_FILENO, "%p", (void*)value);
}
