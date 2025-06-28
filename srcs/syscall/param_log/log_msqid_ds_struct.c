#include "param_log.h"
#include <ft_printf.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

/**
 * @brief Log a msqid_ds struct
 *
 * @param value the value to log
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_MSQID_DS_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct msqid_ds, msgid_ds);
	return ft_dprintf(
		STDERR_FILENO,
		"{msg_perm={uid=%d, gid=%d, mode=%#o, key=%#x, cuid=%d, cgid=%d}, msg_stime=%ld, "
		"msg_rtime=%ld, msg_ctime=%ld, msg_qnum=%ld, msg_qbytes=%ld, msg_lspid=%d, msg_lrpid=%d}",
		msgid_ds.msg_perm.uid, msgid_ds.msg_perm.gid, msgid_ds.msg_perm.mode,
		msgid_ds.msg_perm.__key, msgid_ds.msg_perm.cuid, msgid_ds.msg_perm.cgid, msgid_ds.msg_stime,
		msgid_ds.msg_rtime, msgid_ds.msg_ctime, msgid_ds.msg_qnum, msgid_ds.msg_qbytes,
		msgid_ds.msg_lspid, msgid_ds.msg_lrpid);
}