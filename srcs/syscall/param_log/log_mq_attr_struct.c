#include "param_log.h"
#include <fcntl.h>
#include <ft_printf.h>
#include <mqueue.h>
#include <sys/stat.h>

int log_MQ_ATTR_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct mq_attr, mq_attr);
	return ft_dprintf(STDERR_FILENO, "{mq_flags=%d, mq_maxmsg=%d, mq_msgsize=%d, mq_curmsgs=%d}",
					  mq_attr.mq_flags, mq_attr.mq_maxmsg, mq_attr.mq_msgsize, mq_attr.mq_curmsgs);
}