#define _GNU_SOURCE

#include "param_log.h"
#include <ft_printf.h>
#include <ft_strace_utils.h>
#include <macros.h>
#include <signal.h>
#include <sys/uio.h>

struct kernel_sigaction
{
	__sighandler_t _sa_handler;
	unsigned long sa_flags;
	void (*sa_restorer)(void);
	sigset_t sa_mask;
};

#define SA_RESTORER                                                                                \
	0x04000000 /* since this is in kernel headers we  have to define it ourselves                  \
				*/

static const flag_str_t sa_flags_str[] = {
	FLAG_STR(SA_RESTORER),	FLAG_STR(SA_ONSTACK), FLAG_STR(SA_RESTART),	  FLAG_STR(SA_SIGINFO),
	FLAG_STR(SA_NOCLDSTOP), FLAG_STR(SA_NODEFER), FLAG_STR(SA_RESETHAND), FLAG_STR(SA_NOCLDWAIT),
	FLAG_STR(SA_NOMASK),	FLAG_STR(SA_ONESHOT), FLAG_STR(SA_STACK),
};

static int log_sa_handler(__sighandler_t handler)
{
	if (handler == SIG_DFL)
		return ft_dprintf(STDERR_FILENO, "SIG_DFL");
	if (handler == SIG_IGN)
		return ft_dprintf(STDERR_FILENO, "SIG_IGN");
	return ft_dprintf(STDERR_FILENO, "%p", handler);
}

/**
 * @brief log sigaction struct
 *
 * @param value
 * @return int
 */
int log_SIGACTION_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct kernel_sigaction, sigaction);
	int size_written = 0;
	size_written += ft_dprintf(STDERR_FILENO, "{sa_handler=");
	size_written += log_sa_handler(sigaction._sa_handler);
	size_written += ft_dprintf(STDERR_FILENO, ", sa_mask=");
	size_written += log_local_sigset_struct(&sigaction.sa_mask);
	size_written += ft_dprintf(STDERR_FILENO, ", sa_flags=");
	size_written += flags_log(sigaction.sa_flags, sa_flags_str, ELEM_COUNT(sa_flags_str));
	if (sigaction.sa_flags & SA_RESTORER)
	{
		size_written += ft_dprintf(STDERR_FILENO, ", sa_restorer=");
		size_written += log_PTR((uint64_t)sigaction.sa_restorer);
	}
	size_written += ft_dprintf(STDERR_FILENO, "}");
	return size_written;
}