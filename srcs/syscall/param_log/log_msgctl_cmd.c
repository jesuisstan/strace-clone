#include "param_log.h"
#include <ft_printf.h>
#include <macros.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

static const flag_str_t msgctl_cmd[] = {
	FLAG_STR(IPC_RMID), FLAG_STR(IPC_SET),	FLAG_STR(IPC_STAT),
	FLAG_STR(MSG_INFO), FLAG_STR(MSG_STAT),
};

/**
 * @brief Log shmctl cmd
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_MSGCTL_CMD(uint64_t value)
{
	return option_log(value, msgctl_cmd, ELEM_COUNT(msgctl_cmd), "IPC_???");
}