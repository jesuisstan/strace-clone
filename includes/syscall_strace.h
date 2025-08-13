#ifndef SYSCALL_STRACE_H
#define SYSCALL_STRACE_H

#include <sys/types.h>
#include <sys/user.h>
#include <stdint.h>
#include <types.h>
#include <stdbool.h>

// Forward declarations
struct s_statistics;

// External variables
extern bool execve_printed;
extern bool execve_success;
typedef struct s_failed_execve {
	char *filename;
	unsigned long argv_ptr;
	unsigned long envp_ptr;
	int env_count;
	int arg_count;
	int err;
	bool valid;
} failed_execve_t;
extern failed_execve_t last_failed_execve;
void print_execve(const char *filename, unsigned long envp_ptr, int arg_count, int env_count, long ret, int err);

typedef enum {
	NONE = 0,
	INTEGER,
	STRING,
	POINTER,
	HEX,
	OPEN_FLAGS,
	OPEN_MODE,
	MMAP_FLAGS,
	MEM_PROT,
	ACCESS_MODE,
	SIGNED_INT,
	MEMSEG,
	STAT_STRUCT,
	POLL_FDS,
	POLL_FDS_AFTER,
	SEEK_WHENCE,
	SIGNAL_NAME,
	SIGACTION_STRUCT,
	SIGSET_STRUCT,
	SIGPROCMASK_HOW,
	IOVEC_STRUCT,
	PIPEFDS,
	FD_SET_STRUCT,
	TIMEVAL_STRUCT,
	SELECT_RETURN,
	MREMAP_FLAGS,
	MSYNC_FLAGS,
	MADVISE_ADVISE,
	SHMAT_FLAGS,
	SHMID_DS_STRUCT,
	SHMCTL_CMD,
	KERNEL_TIMESPEC_STRUCT,
	ITIMERVAL_STRUCT,
	ITIMER_WHICH,
	ADDRESS_FAMILY,
	SOCKET_TYPE,
	IPPROTO,
	SOCKADDR_STRUCT,
	SEND_FLAGS,
	MSGHDR_STRUCT,
	INT_PTR,
	SHUTDOWN_HOW,
	ARGV,
	ENVP,
	WAIT_OPTIONS,
	WAIT_STATUS,
	RUSAGE_STRUCT,
	UTSNAME_STRUCT,
	IPCS_FLAGS,
	SEMBUF_STRUCT,
	SEMCTL_CMD,
	MSGBUF_STRUCT,
	MSGFLG,
	MSGCTL_CMD,
	MSQID_DS_STRUCT,
	FCNTL_CMD,
	FLOCK_CMD,
	TIMEZONE_STRUCT,
	RLIMIT_RESOURCE,
	RLIMIT_STRUCT,
	RUSAGE_WHO,
	SYSINFO_STRUCT,
	TMS_STRUCT,
	PTRACE_REQUEST,
	SYSLOG_TYPE,
	INT_ARRAY,
	SIGINFO,
	SIGALTSTACK_STRUCT,
	UTIMBUF_STRUCT,
	PERSONALITY_FLAGS,
	USTAT_STRUCT,
	STATFS_STRUCT,
	PRIORITY_WHICH,
	SCHED_PARAM_STRUCT,
	SCHED_SETSCHEDULER_POLICY,
	MLOCKALL_FLAGS,
	PRCTL_OPTION,
	ARCH_PRCTL_OPTION,
	GETRANDOM_FLAGS,
	IOCTL_CMD,
	KERNEL_TIMEX_STRUCT,
	MOUNT_FLAGS,
	SWAP_FLAGS,
	TIME_T,
	IO_EVENT_STRUCT,
	EPOLL_EVENT_STRUCT,
	EPOLL_CTL_CMD,
	EPOLL_EVENT_STRUCT_ARRAY,
	ADVISE,
	CLOCKID_T,
	SIGEVENT_STRUCT,
	TIMER_SETTIME_FLAGS,
	KERNEL_ITIMERSPEC_STRUCT,
	MQ_ATTR_STRUCT,
	WAITID_WHICH,
	KEYCTL_CMD,
	IOPRIO_WHICH,
	OPENAT_DIRFD,
	RLIMIT64_STRUCT,
	NEWFSTATAT_FLAGS,
	PRLIMIT_RESOURCE,
	UNSHARE_FLAGS,
	CLONE3_STRUCT,
	FUTEX_OP,
	FUTEX_FLAGS,
	SOCKET_LEVEL,
	SOCKET_OPTNAME,
	FD_NUMBER,
	PID_NUMBER,
	EXIT_CODE
} param_type_t;

typedef void (*custom_syscall_handler_t)(pid_t pid,
									   struct user_regs_struct *regs);

typedef struct {
	const char *name;
	param_type_t return_type;
	param_type_t param_types[6];
	custom_syscall_handler_t custom_handler;
} syscall_description_t;

extern const syscall_description_t x86_64_syscalls[];

void syscall_handle(pid_t pid, struct user_regs_struct *regs, bool is_exit);

const syscall_description_t *syscall_get_description(uint64_t syscall_no, architecture_t type);

#endif