#include "param_log.h"
#include <ft_strace_utils.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <stdbool.h>

/**
 * @brief Log sembuf struct
 *
 * @param value the ptr to sembuf
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_SEMBUF_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	int size_written = 0;
	void *ptr = handle_ptr(value, context, &size_written);
	if (ptr == NULL)
		return size_written;
	uint64_t n_sem_ops = 0; // TODO: replace with correct value if needed
	struct sembuf *sops = malloc(sizeof(struct sembuf) * n_sem_ops);
	if (!sops)
	{
		// log_error("log_SEMBUF_STRUCT", "malloc", true); // Uncomment if log_error is available
		return size_written;
	}
	if (remote_memcpy(sops, context->pid, ptr, sizeof(struct sembuf) * n_sem_ops) < 0)
	{
		size_written += printf("%p", ptr);
		free(sops);
		return size_written;
	}
	size_written += printf("[");
	bool first = true;
	for (uint64_t i = 0; i < n_sem_ops; i++)
	{
		if (!first)
			size_written += printf(", ");
		first = false;
		size_written += printf("{.sem_num=%d, .sem_op=%d, .sem_flg=%d}",
							   sops[i].sem_num, sops[i].sem_op, sops[i].sem_flg);
	}
	size_written += printf("]");
	free(sops);
	return size_written;
}