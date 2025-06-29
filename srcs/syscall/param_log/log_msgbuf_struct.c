#include "param_log.h"
#include <stdint.h>

// Stub function for missing register access
static int64_t registers_get_param(void *regs __attribute__((unused)), int type __attribute__((unused)), int index __attribute__((unused))) { return 0; }

struct msgbuf
{
	long mtype;
	char mtext[];
};

/**
 * @brief Log a msgbuf struct
 *
 * @param value the pointer to the msgbuf struct
 * @param context the syscall context
 * @return int the number of bytes written
 */
int log_MSGBUF_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct msgbuf, local_value);
	int size_written = dprintf(STDERR_FILENO, "{mtype=%ld, mtext=", local_value.mtype);
	uint64_t len_mtext =
		registers_get_param(context->regs, context->type, context->arg_index + 1); // msgsz
	size_written += log_memseg_remote(context->pid, &((struct msgbuf *)value)->mtext, len_mtext);
	size_written += dprintf(STDERR_FILENO, "}");
	return size_written;
}