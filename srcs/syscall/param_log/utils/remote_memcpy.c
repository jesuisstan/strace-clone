#define _GNU_SOURCE
#include <sys/uio.h>

/**
 * @brief Copy memory from a process to another
 *
 * @param dest the destination address
 * @param pid the pid of the process to copy from
 * @param src the source address
 * @param len the length of the memory to copy
 * @return int 0 on success, -1 on error
 */
int remote_memcpy(void *dest, pid_t pid, void *src, size_t len)
{
	struct iovec local = {
		.iov_base = dest,
		.iov_len = len,
	};
	struct iovec remote = {
		.iov_base = src,
		.iov_len = len,
	};
	if (process_vm_readv(pid, &local, 1, &remote, 1, 0) < 0)
		return -1;
	return 0;
}