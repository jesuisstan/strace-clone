#define _GNU_SOURCE

#include "param_log.h"
#include <ft_strace_utils.h>
#include <sys/uio.h>
#include <stdbool.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Stub functions for missing register access
static int64_t REGISTERS_GET_RETURN(void *regs __attribute__((unused)), int type __attribute__((unused))) { return 0; }
static int64_t registers_get_param(void *regs __attribute__((unused)), int type __attribute__((unused)), int index __attribute__((unused))) { return 0; }

/**
 * @brief Log iov_base
 *
 * @param pid the pid of the remote process
 * @param iov the iovec struct
 * @param is_fail true if syscall failed
 * @return int the number of bytes written
 */
static int log_iov_base(pid_t pid, struct iovec *iov, bool is_fail, uint64_t len)
{
	if (iov->iov_base == 0)
		return dprintf(STDERR_FILENO, "NULL");
	if (is_fail)
		return dprintf(STDERR_FILENO, "%p", iov->iov_base);
	return log_memseg_remote(pid, iov->iov_base, len);
}

/**
 * @brief Check if syscall failed
 *
 * @param context the context of the syscall
 * @return bool_t true if syscall failed
 */
static bool get_is_fail(syscall_log_param_t *context)
{
	if (context->after_syscall)
		return REGISTERS_GET_RETURN(context->regs, context->type) < 0;
	return false; // always false before syscall
}

/**
 * @brief Get the len needed to log
 *
 * @param iov the iovec struct
 * @param context the context of the syscall
 * @return uint64_t the len needed to log
 */
static uint64_t get_len(struct iovec *iov, int64_t *total_len)
{
	if (*total_len != NO_SIZE)
	{
		int64_t to_log = (iov->iov_len < (uint64_t)*total_len) ? iov->iov_len : (uint64_t)*total_len;
		*total_len -= to_log;
		return to_log;
	}
	return iov->iov_len;
}

/**
 * @brief Log iovec struct in local process but with remote iov_base
 *
 * @param pid the pid of the remote process
 * @param iov the iovec struct
 * @param vlen the number of iovec struct
 * @param total_len the total len read or -1 if not known
 * @param is_fail true if syscall failed
 * @return int the number of bytes written
 */
int log_iovec_struct_local(int pid, struct iovec *iov, uint64_t vlen, int64_t total_len,
						   bool is_fail)
{
	int size_written = 0;
	size_written += dprintf(STDERR_FILENO, "[");
	bool first = true;
	for (uint64_t i = 0; i < vlen; i++)
	{
		if (!first)
			size_written += dprintf(STDERR_FILENO, ", ");
		first = false;
		size_written += dprintf(STDERR_FILENO, "{");
		size_written += dprintf(STDERR_FILENO, ".iov_base=");
		size_written += log_iov_base(pid, &iov[i], is_fail, get_len(&iov[i], &total_len));
		size_written += dprintf(STDERR_FILENO, ", .iov_len=%lu", iov[i].iov_len);
		size_written += dprintf(STDERR_FILENO, "}");
	}
	size_written += dprintf(STDERR_FILENO, "]");
	return size_written;
}

/**
 * @brief Log iovec struct
 *
 * @param value the pointer to the iovec struct in remote process
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_IOVEC_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	if (value == 0)
		return dprintf(STDERR_FILENO, "NULL");
	uint64_t vlen = registers_get_param(context->regs, context->type, context->arg_index + 1);
	struct iovec *iov = malloc(sizeof(struct iovec) * vlen);
	if (iov == NULL)
	{
		fprintf(stderr, "log_IOVEC_STRUCT: malloc failed\n");
		return 0;
	}
	if (remote_memcpy(iov, context->pid, (void *)value, sizeof(struct iovec) * vlen) < 0)
	{
		int size_written = dprintf(STDERR_FILENO, "%p", (void *)value);
		free(iov);
		return size_written;
	}
	int64_t total_len = NO_SIZE;
	if (context->after_syscall)
		total_len = REGISTERS_GET_RETURN(context->regs, context->type);
	int size_written =
		log_iovec_struct_local(context->pid, iov, vlen, total_len, get_is_fail(context));
	free(iov);
	return size_written;
}
