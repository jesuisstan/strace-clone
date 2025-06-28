#include "param_log.h"
#include <ft_printf.h>
#include <macros.h>
#include <sys/sem.h>

static const flag_str_t semctl_cmd[] = {
	FLAG_STR(IPC_RMID), FLAG_STR(IPC_SET), FLAG_STR(IPC_STAT), FLAG_STR(SEM_INFO),
	FLAG_STR(SEM_STAT), FLAG_STR(GETALL),  FLAG_STR(GETNCNT),  FLAG_STR(GETPID),
	FLAG_STR(GETVAL),	FLAG_STR(GETZCNT), FLAG_STR(SETALL),   FLAG_STR(SETVAL),
};

/**
 * @brief Log shmctl cmd
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_SEMCTL_CMD(uint64_t value)
{
	return option_log(value, semctl_cmd, ELEM_COUNT(semctl_cmd), "IPC_???");
}