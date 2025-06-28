#include "param_log.h"
#include <ft_printf.h>
#include <macros.h>
#include <sys/epoll.h>

static const flag_str_t epoll_ctl_cmd_options[] = {
	FLAG_STR(EPOLL_CTL_ADD),
	FLAG_STR(EPOLL_CTL_DEL),
	FLAG_STR(EPOLL_CTL_MOD),
};

int log_EPOLL_CTL_CMD(uint64_t value)
{
	return option_log(value, epoll_ctl_cmd_options, ELEM_COUNT(epoll_ctl_cmd_options),
					  "EPOLL_CTL_???");
}