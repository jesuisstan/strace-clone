#include "param_log.h"
#include <sys/epoll.h>
#include <stdbool.h>

// Stub function for missing register access
static int64_t REGISTERS_GET_RETURN(void *regs __attribute__((unused)), int type __attribute__((unused))) { return 0; }

int log_EPOLL_EVENT_STRUCT_ARRAY(uint64_t value, syscall_log_param_t *context)
{
	int size_written = 0;
	void *remote_ptr = handle_ptr(value, context, &size_written);
	if (remote_ptr == NULL)
		return size_written;
	long event_count = REGISTERS_GET_RETURN(context->regs, context->type);
	struct epoll_event *events = malloc(sizeof(struct epoll_event) * event_count);
	if (events == NULL)
	{
		log_error("log_EPOLL_EPOLL_EVENT_STRUCT_ARRAY", "malloc failed", true);
		return size_written;
	}
	if (remote_memcpy(events, context->pid, remote_ptr, sizeof(struct epoll_event) * event_count) <
		0)
	{
		size_written += dprintf(STDERR_FILENO, "%p", remote_ptr);
		free(events);
		return size_written;
	}
	size_written += dprintf(STDERR_FILENO, "[");
	bool first = true;
	for (long i = 0; i < event_count; i++)
	{
		if (!first)
			size_written += dprintf(STDERR_FILENO, ", ");
		first = false;
		size_written += log_local_epoll_event_struct(&events[i]);
	}
	size_written += dprintf(STDERR_FILENO, "]");
	free(events);
	return size_written;
}