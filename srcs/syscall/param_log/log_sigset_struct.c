#define _GNU_SOURCE

#include "param_log.h"
#include <ft_printf.h>
#include <ft_strace_utils.h>
#include <ft_string.h>
#include <macros.h>
#include <registers.h>
#include <signal.h>
#include <sys/uio.h>

/**
 * @brief Skip the SIG prefix
 *
 * @param str the string to skip
 * @return const char* the string without the SIG prefix
 */
const char *skip_sigprefix(const char *str)
{
	if (ft_memcmp(str, "SIG", 3) == 0)
		return str + 3;
	return str;
}


#define NB_SIG 65

/**
 * @brief Log sigset in local memory
 *
 * @param sigset
 * @return int
 */
int log_local_sigset_struct(sigset_t *sigset)
{
	int size_written = ft_dprintf(STDERR_FILENO, "[");
	bool_t first = true;
	for (unsigned i = 2; i < NB_SIG; i++)
	{
		if (sigismember(sigset, i))
		{
			if (!first)
				size_written += ft_dprintf(STDERR_FILENO, " ");
			first = false;
			size_written += ft_dprintf(STDERR_FILENO, "%s", skip_sigprefix(ft_signalname(i)));
		}
	}
	size_written += ft_dprintf(STDERR_FILENO, "]");
	return size_written;
}

/**
 * @brief log a sigset_t struct
 *
 * @param value
 * @param context
 * @return int
 */
int log_SIGSET_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(sigset_t, sigset);
	return log_local_sigset_struct(&sigset);
}