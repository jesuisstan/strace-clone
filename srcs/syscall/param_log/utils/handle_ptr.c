#include "../param_log.h"
#include <stdint.h>

// Stub function for missing register access
static int64_t REGISTERS_GET_RETURN(void *regs __attribute__((unused)), int type __attribute__((unused))) { return 0; }

/**
 * @brief Handle a pointer value
 *
 * @param value the pointer value
 * @param context the syscall context
 * @param size_written the number of bytes written
 * @return void*
 */
void *handle_ptr(uint64_t value, syscall_log_param_t *context, int *size_written)
{
	if (value == 0)
	{
		*size_written += dprintf(STDERR_FILENO, "NULL");
		return NULL;
	}
	if (context->after_syscall)
	{
		int64_t ret = REGISTERS_GET_RETURN(context->regs, context->type);
		if (ret < 0)
		{
			*size_written += dprintf(STDERR_FILENO, "%p", (void *)value);
			return NULL;
		}
	}
	return (void *)value;
}