#include "param_log.h"
#include <ft_printf.h>
#include <ft_strace_utils.h>
#include <macros.h>
#include <signal.h>
#include <sys/uio.h>

int log_SIGALTSTACK_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(stack_t, stack);
	return ft_dprintf(STDERR_FILENO, "{ss_sp=%p, ss_flags=%#x, ss_size=%lu}", stack.ss_sp,
					  stack.ss_flags, stack.ss_size);
}