#include "param_log.h"
#include <ft_printf.h>

struct io_event
{
	uint64_t data;
	uint64_t obj;
	int64_t res;
	int64_t res2;
};

int log_IO_EVENT_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	int size_written = 0;
	void *remote_ptr = handle_ptr(value, context, &size_written);
	if (remote_ptr == NULL)
		return size_written;
	long event_count = REGISTERS_GET_RETURN(context->regs, context->type);
	struct io_event *events = malloc(sizeof(struct io_event) * event_count);
	if (events == NULL)
	{
		log_error("log_IO_EVENT_STRUCT", "malloc failed", true);
		return size_written;
	}
	if (remote_memcpy(events, context->pid, remote_ptr, sizeof(struct io_event) * event_count) < 0)
	{
		size_written += ft_dprintf(STDERR_FILENO, "%p", remote_ptr);
		free(events);
		return size_written;
	}
	size_written += ft_dprintf(STDERR_FILENO, "[");
	bool_t first = true;
	for (long i = 0; i < event_count; i++)
	{
		if (!first)
			size_written += ft_dprintf(STDERR_FILENO, ", ");
		first = false;
		size_written += ft_dprintf(STDERR_FILENO, "{.data=%lu, .obj=%lu, .res=%ld, .res2=%ld}",
								   events[i].data, events[i].obj, events[i].res, events[i].res2);
	}
	size_written += ft_dprintf(STDERR_FILENO, "]");
	free(events);
	return size_written;
}