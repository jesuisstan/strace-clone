#include "param_log.h"
#include <ft_printf.h>
#include <ft_strace_utils.h>
#include <ft_string.h>
#include <macros.h>
#include <signal.h>

/**
 * @brief Log siginfo struct
 *
 * @param value the pointer to the siginfo struct
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_SIGINFO(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(siginfo_t, siginfo);
	int size_written = 0;
	size_written += ft_dprintf(
		STDERR_FILENO, "{si_signo=%s, si_code=%#x, si_pid=%d, si_uid=%d, si_int=%d, si_ptr=%p}",
		ft_signalname(siginfo.si_signo), siginfo.si_code, siginfo.si_pid, siginfo.si_uid,
		siginfo.si_int, (void *)siginfo.si_ptr);
	return size_written;
}