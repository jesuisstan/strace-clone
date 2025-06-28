#define _GNU_SOURCE
#include "param_log.h"
#include <ft_printf.h>
#include <ft_strace_utils.h>
#include <macros.h>
#include <sys/shm.h>
#include <sys/uio.h>

/**
 * @brief Log shmid_ds struct
 *
 * @param value the value to log
 * @param context the syscall_log_param_t struct
 * @return int the number of bytes written
 */
int log_SHMID_DS_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct shmid_ds, shmid_ds);
	return ft_dprintf(
		STDERR_FILENO,
		"{shm_perm={uid=%d, gid=%d, mode=%#o, key=%d, cuid=%d, cgid=%d}, shm_segsz=%llu, "
		"shm_cpid=%d, shm_lpid=%d, shm_nattch=%d, shm_atime=%d, shm_dtime=%d, shm_ctime=%d}",
		shmid_ds.shm_perm.uid, shmid_ds.shm_perm.gid, shmid_ds.shm_perm.mode,
		shmid_ds.shm_perm.__key, shmid_ds.shm_perm.cuid, shmid_ds.shm_perm.cgid, shmid_ds.shm_segsz,
		shmid_ds.shm_cpid, shmid_ds.shm_lpid, shmid_ds.shm_nattch, shmid_ds.shm_atime,
		shmid_ds.shm_dtime, shmid_ds.shm_ctime);
}
