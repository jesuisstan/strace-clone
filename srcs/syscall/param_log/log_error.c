#include "ft_strace_utils.h"
#include "config.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief show a error message in STDERR
 *
 * @param context the context where the error was raised
 * @param message the error message
 * @param show_error if true, the message derivate from errno will be shown
 */
void log_error(const char *context, const char *message, int show_error)
{
	dprintf(STDERR_FILENO, "ft_strace: %s: %s", context, message);
	
	if (show_error) {
	int saved_errno = errno;
		dprintf(STDERR_FILENO, ": %s", strerror(saved_errno));
	}
	
	dprintf(STDERR_FILENO, "\n");
}