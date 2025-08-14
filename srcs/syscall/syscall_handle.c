#include <syscall_strace.h>
#include <ft_strace_utils.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdbool.h>
#include "param_log/param_log.h"
#include <stdint.h>
#include "types.h"

// Function prototypes
static char *read_string_via_proc(pid_t pid, unsigned long addr);

// Global variable to track current syscall number
char *execve_filename = NULL;

// Global variable to track statistics mode
extern bool g_statistics_mode;

bool execve_printed = false;
bool execve_success = false;
failed_execve_t last_failed_execve = {0};

// --- For execve: save parameters on entry ---
static char *execve_filename_in = NULL;
static unsigned long execve_argv_in = 0;
static unsigned long execve_envp_in = 0;
static int execve_argc_in = 0;
static int execve_envc_in = 0;
static char **execve_argv_saved = NULL;  // Save actual argument strings

void print_execve(const char *filename, unsigned long envp_ptr, int arg_count, int env_count, long ret, int err) {
	dprintf(STDERR_FILENO, "execve(\"%s\", [", filename);
	for (int i = 0; i < arg_count; ++i) {
		if (i > 0) dprintf(STDERR_FILENO, ", ");
		if (execve_argv_saved && execve_argv_saved[i]) {
			dprintf(STDERR_FILENO, "\"%s\"", execve_argv_saved[i]);
		} else {
			dprintf(STDERR_FILENO, "(null)");
		}
	}
	dprintf(STDERR_FILENO, "] , %p /* %d vars */) = ", (void*)envp_ptr, env_count);
	if (ret < 0) {
		dprintf(STDERR_FILENO, "-1 %s (%s)\n", ft_errnoname(err), strerror(err));
	} else {
		dprintf(STDERR_FILENO, "%ld\n", ret);
	}
}

// Helper to print pointer/special values in strace style
static void print_ptr_special(unsigned long long value) {
	if (value == 0) {
		dprintf(STDERR_FILENO, "NULL");
	} else if (value == (unsigned long long)-1 || value == 0xFFFFFFFFFFFFFFFFull || value == 0xFFFFFFFFul) {
		dprintf(STDERR_FILENO, "-1");
	} else if (value == 0xFFFFFFFFFFFFFF9Cull || value == 0xFFFFFF9Cul) {
		dprintf(STDERR_FILENO, "AT_FDCWD");
	} else {
		dprintf(STDERR_FILENO, "0x%llx", value);
	}
}

#define CSIGNAL 0x000000ff
#define CLONE_VM 0x00000100
#define CLONE_FS 0x00000200
#define CLONE_FILES 0x00000400
#define CLONE_SIGHAND 0x00000800
#define CLONE_PIDFD 0x00001000
#define CLONE_PTRACE 0x00002000
#define CLONE_VFORK 0x00004000
#define CLONE_PARENT 0x00008000
#define CLONE_THREAD 0x00010000
#define CLONE_NEWNS 0x00020000
#define CLONE_SYSVSEM 0x00040000
#define CLONE_SETTLS 0x00080000
#define CLONE_PARENT_SETTID 0x00100000
#define CLONE_CHILD_CLEARTID 0x00200000
#define CLONE_DETACHED 0x00400000
#define CLONE_UNTRACED 0x00800000
#define CLONE_CHILD_SETTID 0x01000000
#define CLONE_NEWCGROUP 0x02000000
#define CLONE_NEWUTS 0x04000000
#define CLONE_NEWIPC 0x08000000
#define CLONE_NEWUSER 0x10000000
#define CLONE_NEWPID 0x20000000
#define CLONE_NEWNET 0x40000000
#define CLONE_IO 0x80000000

static const flag_str_t clone_flags[] = {
    FLAG_STR(CLONE_VM), FLAG_STR(CLONE_FS), FLAG_STR(CLONE_FILES),
    FLAG_STR(CLONE_SIGHAND), FLAG_STR(CLONE_PIDFD), FLAG_STR(CLONE_PTRACE),
    FLAG_STR(CLONE_VFORK), FLAG_STR(CLONE_PARENT), FLAG_STR(CLONE_THREAD),
    FLAG_STR(CLONE_NEWNS), FLAG_STR(CLONE_SYSVSEM), FLAG_STR(CLONE_SETTLS),
    FLAG_STR(CLONE_PARENT_SETTID), FLAG_STR(CLONE_CHILD_CLEARTID),
    FLAG_STR(CLONE_DETACHED), FLAG_STR(CLONE_UNTRACED),
    FLAG_STR(CLONE_CHILD_SETTID), FLAG_STR(CLONE_NEWCGROUP),
    FLAG_STR(CLONE_NEWUTS), FLAG_STR(CLONE_NEWIPC), FLAG_STR(CLONE_NEWUSER),
    FLAG_STR(CLONE_NEWPID), FLAG_STR(CLONE_NEWNET), FLAG_STR(CLONE_IO),
};

static void print_clone_details(pid_t pid, struct user_regs_struct *regs)
{
    (void)pid;
	uint64_t child_stack = regs->rsi;
	uint64_t flags = regs->rdi;
	uint64_t child_tidptr = regs->rdx;

	dprintf(STDERR_FILENO, "child_stack=");
    log_PTR(child_stack);
    dprintf(STDERR_FILENO, ", flags=");
    flags_log(flags, clone_flags, sizeof(clone_flags) / sizeof(clone_flags[0]));
    dprintf(STDERR_FILENO, ", child_tidptr=");
    log_PTR(child_tidptr);
}

/**
 * @brief Handle syscall logging
 *
 * @param pid
 * @param regs
 * @param is_exit
 */
void syscall_handle(pid_t pid, struct user_regs_struct *regs, bool is_exit)
{
	if (!regs) {
		return;
	}
	int syscall_no = (int)regs->orig_rax;

	// On entry to execve, save parameters for later use (since memory will be replaced after execve)
	if (syscall_no == 59 && !is_exit) {
		if (execve_filename_in) { free(execve_filename_in); execve_filename_in = NULL; }
		if (execve_argv_saved) {
			for (int i = 0; i < execve_argc_in; i++) {
				if (execve_argv_saved[i]) free(execve_argv_saved[i]);
			}
			free(execve_argv_saved);
			execve_argv_saved = NULL;
		}
		
		execve_filename_in = read_string_via_proc(pid, regs->rdi);
		execve_argv_in = regs->rsi;
		execve_envp_in = regs->rdx;
		
		// Count arguments and save them
		execve_argc_in = 0;
		if (execve_argv_in) {
			while (1) {
				unsigned long ptr = 0;
				if (remote_memcpy(&ptr, pid, (void*)(execve_argv_in + execve_argc_in * sizeof(unsigned long)), sizeof(unsigned long)) < 0) {
					break;
				}
				if (ptr == 0) break;
				execve_argc_in++;
			}
		}
		
		// Save actual argument strings
		if (execve_argc_in > 0) {
			execve_argv_saved = malloc(execve_argc_in * sizeof(char*));
			if (execve_argv_saved) {
				for (int i = 0; i < execve_argc_in; i++) {
					unsigned long ptr = 0;
					if (remote_memcpy(&ptr, pid, (void*)(execve_argv_in + i * sizeof(unsigned long)), sizeof(unsigned long)) >= 0) {
						execve_argv_saved[i] = read_string_via_proc(pid, ptr);
					}
				}
			}
		}
		
		execve_envc_in = 0;
		if (execve_envp_in) {
			while (1) {
				unsigned long ptr = 0;
				if (remote_memcpy(&ptr, pid, (void*)(execve_envp_in + execve_envc_in * sizeof(unsigned long)), sizeof(unsigned long)) < 0) {
					break;
				}
				if (ptr == 0) break;
				execve_envc_in++;
			}
		}
		
		return;
	}

	if (!is_exit)
		return;
	
	if (g_statistics_mode) {
		return;
	}
	
	const char *name = syscall_get_description(syscall_no, X86_64)->name;
	if (!name) name = "unknown";
	
	// Try to get parameter types
	const syscall_description_t *desc = syscall_get_description(syscall_no, X86_64);
	
	unsigned long long args[6] = {
		regs->rdi,
		regs->rsi,
		regs->rdx,
		regs->r10,
		regs->r8,
		regs->r9
	};
	
	// Filtered execve output: print only the first successful execve or the last failed one
	if (syscall_no == 59) {
		long ret = regs->rax;
		int err = -ret;
		if (ret == 0 && !execve_success) {
			// First successful execve
			print_execve(execve_filename_in ? execve_filename_in : "(error)", execve_envp_in, execve_argc_in, execve_envc_in, ret, err);
			execve_success = true;
			if (last_failed_execve.filename) { free(last_failed_execve.filename); last_failed_execve.filename = NULL; }
		} else if (ret < 0) {
			// Save the last failed execve
			if (last_failed_execve.filename) free(last_failed_execve.filename);
			last_failed_execve.filename = execve_filename_in ? strdup(execve_filename_in) : NULL;
			last_failed_execve.argv_ptr = execve_argv_in;
			last_failed_execve.envp_ptr = execve_envp_in;
			last_failed_execve.arg_count = execve_argc_in;
			last_failed_execve.env_count = execve_envc_in;
			last_failed_execve.err = err;
			last_failed_execve.valid = true;
		}
		if (execve_filename_in) { free(execve_filename_in); execve_filename_in = NULL; }
		return;
	}
	
	if (syscall_no == 56) { // clone
		dprintf(STDERR_FILENO, "clone(");
		print_clone_details(pid, regs);
	} else {
		dprintf(STDERR_FILENO, "%s(", name);
	}
	bool printed_arg = false;
	for (int i = 0; i < 6; ++i) {
		if (!desc || desc->param_types[i] == NONE) break;
		syscall_log_param_t context = { .pid = pid, .regs = regs, .arg_index = i };
		param_type_t type = desc->param_types[i];
		bool should_print = true;
		
		// Determine if this argument should be printed
		switch (type < 0 ? -type : type) {
			case OPEN_FLAGS:
				should_print = (args[i] != 0);
				break;
			case OPEN_MODE:
				should_print = (args[i] != 0);
				break;
			default:
				should_print = true;
				break;
		}
		
		if (should_print) {
			if (printed_arg) {
				dprintf(STDERR_FILENO, ", ");
			}
			printed_arg = true;
			
			switch (type < 0 ? -type : type) {
			case STRING: {
				char *str = read_string_via_proc(pid, args[i]);
				if (str) {
					dprintf(STDERR_FILENO, "\"%s\"", str);
					free(str);
				} else {
					dprintf(STDERR_FILENO, "NULL");
				}
				break;
			}
			case POINTER: log_PTR(args[i]); break;
			case HEX: log_HEX(args[i]); break;
			case OPEN_FLAGS:
				log_OPEN_FLAGS(args[i]);
				break;
			case OPEN_MODE:
				log_OPEN_MODE(args[i]);
				break;
			case MMAP_FLAGS: log_MMAP_FLAGS(args[i]); break;
			case MEM_PROT: log_MEM_PROT(args[i]); break;
			case ACCESS_MODE: log_ACCESS_MODE(args[i]); break;
			case SIGNED_INT: log_SIGNED_INT((int64_t)args[i]); break;
			case MEMSEG: log_MEMSEG(args[i], &context); break;
			case STAT_STRUCT: log_STAT_STRUCT(args[i], &context); break;
			case POLL_FDS: log_POLL_FDS(args[i], &context); break;
			case POLL_FDS_AFTER: log_POLL_FDS_AFTER(args[i], &context); break;
			case SEEK_WHENCE: log_SEEK_WHENCE(args[i]); break;
			case SIGNAL_NAME: log_SIGNAL_NAME(args[i]); break;
			case SIGACTION_STRUCT: log_SIGACTION_STRUCT(args[i], &context); break;
			case SIGSET_STRUCT: log_SIGSET_STRUCT(args[i], &context); break;
			case SIGPROCMASK_HOW: log_SIGPROCMASK_HOW(args[i]); break;
			case IOVEC_STRUCT: log_IOVEC_STRUCT(args[i], &context); break;
			case PIPEFDS: log_PIPEFDS(args[i], &context); break;
			case FD_SET_STRUCT: log_FD_SET_STRUCT(args[i], &context); break;
			case TIMEVAL_STRUCT: log_TIMEVAL_STRUCT(args[i], &context); break;
			case SELECT_RETURN: log_SELECT_RETURN(args[i], &context); break;
			case MREMAP_FLAGS: log_MREMAP_FLAGS(args[i]); break;
			case MSYNC_FLAGS: log_MSYNC_FLAGS(args[i]); break;
			case MADVISE_ADVISE: log_MADVISE_ADVISE(args[i]); break;
			case SHMAT_FLAGS: log_SHMAT_FLAGS(args[i]); break;
			case SHMID_DS_STRUCT: log_SHMID_DS_STRUCT(args[i], &context); break;
			case SHMCTL_CMD: log_SHMCTL_CMD(args[i]); break;
			case KERNEL_TIMESPEC_STRUCT: log_KERNEL_TIMESPEC_STRUCT(args[i], &context); break;
			case ITIMERVAL_STRUCT: log_ITIMERVAL_STRUCT(args[i], &context); break;
			case ITIMER_WHICH: log_ITIMER_WHICH(args[i]); break;
			case ADDRESS_FAMILY: log_ADDRESS_FAMILY(args[i]); break;
			case SOCKET_TYPE: log_SOCKET_TYPE(args[i]); break;
			case IPPROTO: log_IPPROTO(args[i]); break;
			case SOCKADDR_STRUCT: log_SOCKADDR_STRUCT(args[i], &context); break;
			case SEND_FLAGS: log_SEND_FLAGS(args[i]); break;
			case MSGHDR_STRUCT: log_MSGHDR_STRUCT(args[i], &context); break;
			case INT_PTR: log_INT_PTR(args[i], &context); break;
			case SHUTDOWN_HOW: log_SHUTDOWN_HOW(args[i]); break;
			case ARGV: log_ARGV(args[i], &context); break;
			case ENVP: log_ENVP(args[i], &context); break;
			case WAIT_OPTIONS: log_WAIT_OPTIONS(args[i]); break;
			case WAIT_STATUS: log_WAIT_STATUS(args[i], &context); break;
			case RUSAGE_STRUCT: log_RUSAGE_STRUCT(args[i], &context); break;
			case UTSNAME_STRUCT: log_UTSNAME_STRUCT(args[i], &context); break;
			case IPCS_FLAGS: log_IPCS_FLAGS(args[i]); break;
			case SEMBUF_STRUCT: log_SEMBUF_STRUCT(args[i], &context); break;
			case SEMCTL_CMD: log_SEMCTL_CMD(args[i]); break;
			case MSGBUF_STRUCT: log_MSGBUF_STRUCT(args[i], &context); break;
			case MSGFLG: log_MSGFLG(args[i]); break;
			case MSGCTL_CMD: log_MSGCTL_CMD(args[i]); break;
			case UNSHARE_FLAGS: flags_log(args[i], clone_flags, sizeof(clone_flags) / sizeof(clone_flags[0])); break;
			case OPENAT_DIRFD:
				log_OPENAT_DIRFD(args[i]);
				break;
			case PRCTL_OPTION:
				log_PRCTL_OPTION(args[i]);
				break;
			case ARCH_PRCTL_OPTION:
				log_ARCH_PRCTL_OPTION(args[i]);
				break;
			case PRLIMIT_RESOURCE:
				log_PRLIMIT_RESOURCE(args[i]);
				break;
			case RLIMIT64_STRUCT:
				log_RLIMIT64_STRUCT(args[i], &context);
				break;
			case GETRANDOM_FLAGS:
				log_GETRANDOM_FLAGS(args[i]);
				break;
			case IOCTL_CMD:
				log_IOCTL_CMD(args[i]);
				break;
			case ADVISE:
				log_ADVISE(args[i]);
				break;
			case CLONE3_STRUCT:
				log_CLONE3_STRUCT(args[i], &context);
				break;
			case FUTEX_OP:
				log_FUTEX_OP(args[i]);
				break;
			case FUTEX_FLAGS:
				log_FUTEX_FLAGS(args[i]);
				break;
			case SOCKET_LEVEL:
				log_SOCKET_LEVEL(args[i]);
				break;
			case SOCKET_OPTNAME:
				log_SOCKET_OPTNAME(args[i]);
				break;
			case FD_NUMBER:
				log_FD_NUMBER(args[i]);
				break;
			case PID_NUMBER:
				log_PID_NUMBER(args[i]);
				break;
			case EXIT_CODE:
				log_EXIT_CODE(args[i]);
				break;
			case RT_SIGRETURN_STRUCT:
				log_RT_SIGRETURN_STRUCT(args[i], &context);
				break;
			default: dprintf(STDERR_FILENO, "%lld", args[i]);
		}
		}
	}
	dprintf(STDERR_FILENO, ")");

	long ret = regs->rax;
	// Print return value for pointer-returning syscalls as hex or special
	if (strcmp(name, "brk") == 0 || strcmp(name, "mmap") == 0 || strcmp(name, "mmap2") == 0) {
		if (ret < 0) {
			int err = -ret;
			dprintf(STDERR_FILENO, " = -1 %s (%s)\n", ft_errnoname(err), strerror(err));
		} else {
			dprintf(STDERR_FILENO, " = ");
			print_ptr_special((unsigned long long)ret);
			dprintf(STDERR_FILENO, "\n");
		}
	} else if (ret < 0) {
		int err = -ret;
		dprintf(STDERR_FILENO, " = -1 %s (%s)\n", ft_errnoname(err), strerror(err));
	} else {
		dprintf(STDERR_FILENO, " = %ld\n", ret);
	}
}

/**
 * @brief Read a string from traced process memory using /proc/pid/mem
 *
 * @param pid
 * @param addr
 * @return char* allocated string or NULL if error
 */
static char *read_string_via_proc(pid_t pid, unsigned long addr)
{
	if (addr == 0) {
		return strdup("(null)");
	}
	
	char proc_mem_path[64];
	snprintf(proc_mem_path, sizeof(proc_mem_path), "/proc/%d/mem", pid);
	
	int fd = open(proc_mem_path, O_RDONLY);
	if (fd == -1) {
		return strdup("(error)");
	}
	
	// Seek to the address
	if (lseek(fd, addr, SEEK_SET) == -1) {
		close(fd);
		return strdup("(error)");
	}

	// Read string in chunks
	char *str = malloc(4096);
	if (!str) {
		close(fd);
		return strdup("(error)");
	}
	
	memset(str, 0, 4096);
	
	// Read up to 4095 bytes (leave space for null terminator)
	ssize_t bytes_read = read(fd, str, 4095);
	close(fd);
	
	if (bytes_read == -1) {
		free(str);
		return strdup("(error)");
	}
	
	// Ensure null termination
	str[bytes_read] = '\0';
	
	// Find the actual string length (up to first null byte)
	size_t len = strlen(str);
	if (len > 0) {
		// Truncate if too long
		if (len > 100) {
			str[100] = '\0';
			len = 100;
		}
		
		// Escape special characters
		char *escaped = malloc(len * 4 + 1);
		if (!escaped) {
			free(str);
			return strdup("(error)");
		}
		
		size_t j = 0;
		for (size_t i = 0; i < len; i++) {
			if (str[i] >= 32 && str[i] <= 126) {
				escaped[j++] = str[i];
			} else {
				switch (str[i]) {
					case '\t': escaped[j++] = '\\'; escaped[j++] = 't'; break;
					case '\n': escaped[j++] = '\\'; escaped[j++] = 'n'; break;
					case '\r': escaped[j++] = '\\'; escaped[j++] = 'r'; break;
					default: 
						sprintf(escaped + j, "\\%03o", (unsigned char)str[i]);
						j += 4;
						break;
				}
			}
		}
		escaped[j] = '\0';
		
		free(str);
		return escaped;
	}
	
	free(str);
	return strdup("(empty)");
}