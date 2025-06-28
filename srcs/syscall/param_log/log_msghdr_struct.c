#define _GNU_SOURCE

#include "param_log.h"
#include <ft_printf.h>
#include <ft_strace_utils.h>
#include <macros.h>
#include <netinet/in.h>
#include <registers.h>
#include <sys/socket.h>
#include <sys/uio.h>

static int log_remote_iovec_struct(void *iov_remote, uint64_t vlen, pid_t pid, int64_t total_len)
{
	struct iovec *iov = malloc(sizeof(struct iovec) * vlen);
	if (iov == NULL)
	{
		log_error("log_remote_iovec_struct", "malloc failed", true);
		return 0;
	}
	struct iovec local = {
		.iov_base = iov,
		.iov_len = sizeof(struct iovec) * vlen,
	};
	struct iovec remote = {
		.iov_base = iov_remote,
		.iov_len = sizeof(struct iovec) * vlen,
	};
	if (process_vm_readv(pid, &local, 1, &remote, 1, 0) < 0)
	{
		log_error("log_remote_iovec_struct", "process_vm_readv failed", true);
		free(iov);
		return 0;
	}
	int size_written = 0;
	size_written += log_iovec_struct_local(pid, iov, vlen, total_len, false);
	free(iov);
	return size_written;
}

/**
 * @brief Log a msghdr struct
 *
 * @param value the ptr to msghdr
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_MSGHDR_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	if (value == 0)
		return ft_dprintf(STDERR_FILENO, "NULL");
	int total_len = NO_SIZE;
	if (context->after_syscall)
	{
		int64_t ret = REGISTERS_GET_RETURN(context->regs, context->type);
		if (ret < 0)
			return ft_dprintf(STDERR_FILENO, "%p", (void *)value);
		total_len = ret;
	}
	else
		total_len =
			(int64_t)registers_get_param(context->regs, context->type, context->arg_index + 1);
	struct msghdr msghdr;
	if (remote_memcpy(&msghdr, context->pid, (void *)value, sizeof(struct msghdr)) < 0)
		return ft_dprintf(STDERR_FILENO, "%p", (void *)value);
	int size_written = 0;
	size_written += ft_dprintf(STDERR_FILENO, "{msg_name=");
	size_written += log_SOCKADDR_STRUCT((uint64_t)msghdr.msg_name, context);
	size_written += ft_dprintf(STDERR_FILENO, ", msg_namelen=%u, msg_iov=", msghdr.msg_namelen);
	size_written +=
		log_remote_iovec_struct(msghdr.msg_iov, msghdr.msg_iovlen, context->pid, total_len);
	size_written += ft_dprintf(
		STDERR_FILENO, ", msg_iovlen=%u, msg_control=%p, msg_controllen=%u, msg_flags=%u}",
		msghdr.msg_iovlen, msghdr.msg_control, msghdr.msg_controllen, msghdr.msg_flags);
	return size_written;
}