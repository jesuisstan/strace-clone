#include "param_log.h"
#include <sys/epoll.h>

static const flag_str_t epoll_ctl_cmd_options[] = {
	FLAG_STR(EPOLL_CTL_ADD),
	FLAG_STR(EPOLL_CTL_DEL),
	FLAG_STR(EPOLL_CTL_MOD),
};

int log_EPOLL_CTL_CMD(uint64_t value)
{
	return option_log(value, epoll_ctl_cmd_options, sizeof(epoll_ctl_cmd_options) / sizeof(epoll_ctl_cmd_options[0]),
					  "EPOLL_CTL_???");
}