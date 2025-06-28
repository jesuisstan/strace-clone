#include "param_log.h"
#include <ft_printf.h>
#include <macros.h>
#include <sys/epoll.h>

static const flag_str_t events_flags[] = {
	FLAG_STR(EPOLLIN),	   FLAG_STR(EPOLLPRI),	   FLAG_STR(EPOLLOUT),	  FLAG_STR(EPOLLRDNORM),
	FLAG_STR(EPOLLRDBAND), FLAG_STR(EPOLLWRNORM),  FLAG_STR(EPOLLWRBAND), FLAG_STR(EPOLLMSG),
	FLAG_STR(EPOLLERR),	   FLAG_STR(EPOLLHUP),	   FLAG_STR(EPOLLRDHUP),  FLAG_STR(EPOLLEXCLUSIVE),
	FLAG_STR(EPOLLWAKEUP), FLAG_STR(EPOLLONESHOT), FLAG_STR(EPOLLET),
};

int log_local_epoll_event_struct(struct epoll_event *event)
{
	int size_written = ft_dprintf(STDERR_FILENO, "{.events=");
	size_written += flags_log(event->events, events_flags, ELEM_COUNT(events_flags));
	size_written += ft_dprintf(STDERR_FILENO, ", .data=");
	if (event->events & EPOLLIN || event->events & EPOLLPRI || event->events & EPOLLRDNORM ||
		event->events & EPOLLRDBAND)
		size_written += ft_dprintf(STDERR_FILENO, "%d", event->data.fd);
	else
		size_written += ft_dprintf(STDERR_FILENO, "%lu", event->data.u64);
	size_written += ft_dprintf(STDERR_FILENO, "}");
	return size_written;
}

int log_EPOLL_EVENT_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct epoll_event, event);
	if (context->is_return_log && event.events == 0 && event.data.u64 == 0)
		return ft_dprintf(STDERR_FILENO, "(Timeout)");
	return log_local_epoll_event_struct(&event);
}