#include "param_log.h"
#include <ft_printf.h>
#include <macros.h>
#include <sys/shm.h>

static const flag_str_t shmctl_cmd[] = {
	FLAG_STR(IPC_RMID),	  FLAG_STR(IPC_SET),  FLAG_STR(IPC_STAT), FLAG_STR(SHM_LOCK),
	FLAG_STR(SHM_UNLOCK), FLAG_STR(SHM_STAT), FLAG_STR(SHM_INFO),
};

/**
 * @brief Log shmctl cmd
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_SHMCTL_CMD(uint64_t value)
{
	return option_log(value, shmctl_cmd, ELEM_COUNT(shmctl_cmd), "IPC_???");
}