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

// Function prototypes
static char *read_string_via_proc(pid_t pid, unsigned long addr);

// Global variable to track current syscall number
static int current_syscall_no = -1;
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

void print_execve(pid_t pid, const char *filename, unsigned long argv_ptr, unsigned long envp_ptr, int arg_count, int env_count, long ret, int err) {
	printf("execve(\"%s\", [", filename);
	for (int i = 0; i < arg_count; ++i) {
		unsigned long ptr = ptrace(PTRACE_PEEKDATA, pid, argv_ptr + i * sizeof(unsigned long), NULL);
		if (ptr == 0) break;
		if (i > 0) printf(", ");
		char *arg_str = read_string_via_proc(pid, ptr);
		if (arg_str) {
			printf("\"%s\"", arg_str);
			free(arg_str);
		} else {
			printf("(null)");
		}
	}
	printf("] , %p /* %d vars */) = ", (void*)envp_ptr, env_count);
	if (ret < 0) {
		printf("-1 (%s)\n", strerror(err));
	} else {
		printf("%ld\n", ret);
	}
}

// Helper to print pointer/special values in strace style
static void print_ptr_special(unsigned long long value) {
	if (value == 0) {
		printf("NULL");
	} else if (value == (unsigned long long)-1 || value == 0xFFFFFFFFFFFFFFFFull || value == 0xFFFFFFFFul) {
		printf("-1");
	} else if (value == 0xFFFFFFFFFFFFFF9Cull || value == 0xFFFFFF9Cul) {
		printf("AT_FDCWD");
	} else {
		printf("0x%llx", value);
	}
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
		execve_filename_in = read_string_via_proc(pid, regs->rdi);
		execve_argv_in = regs->rsi;
		execve_envp_in = regs->rdx;
		// Count arguments
		execve_argc_in = 0;
		if (execve_argv_in) {
			while (1) {
				unsigned long ptr = ptrace(PTRACE_PEEKDATA, pid, execve_argv_in + execve_argc_in * sizeof(unsigned long), NULL);
				if (ptr == 0) break;
				execve_argc_in++;
			}
		}
		execve_envc_in = 0;
		if (execve_envp_in) {
			while (1) {
				unsigned long ptr = ptrace(PTRACE_PEEKDATA, pid, execve_envp_in + execve_envc_in * sizeof(unsigned long), NULL);
				if (ptr == 0) break;
				execve_envc_in++;
			}
		}
		return;
	}

	if (!is_exit)
		return;
	
	// Special handling for execve on entry
	if (syscall_no == 59) { // execve
		current_syscall_no = syscall_no;
		if (execve_filename) {
			free(execve_filename);
		}
		execve_filename = read_string_via_proc(pid, regs->rdi);
		if (!execve_filename) {
			execve_filename = strdup("(error)");
		}
	}
	
	if (g_statistics_mode) {
		return;
	}
	
	const char *name = syscall_get_description(syscall_no);
	if (!name) name = "unknown";
	
	// Try to get parameter types
	extern const syscall_description_t x86_64_syscalls[];
	const syscall_description_t *desc = NULL;
	if (syscall_no >= 0 && syscall_no < 335) {
		desc = &x86_64_syscalls[syscall_no];
	}
	
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
			print_execve(pid, execve_filename_in ? execve_filename_in : "(error)", execve_argv_in, execve_envp_in, execve_argc_in, execve_envc_in, ret, err);
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
	
	printf("%s(", name);
	for (int i = 0; i < 6; ++i) {
		if (!desc || desc->param_types[i] == NONE) break;
		if (i > 0) printf(", ");
		switch (desc->param_types[i]) {
			case STRING: {
				char *str = read_string_via_proc(pid, args[i]);
				if (str) {
					printf("\"%s\"", str);
					free(str);
				} else {
					printf("(null)");
				}
				break;
			}
			case POINTER:
				print_ptr_special(args[i]);
				break;
			case HEX:
				printf("0x%llx", args[i]);
				break;
			case OPEN_FLAGS:
				log_OPEN_FLAGS(args[i]);
				break;
			case OPEN_MODE:
				log_OPEN_MODE(args[i]);
				break;
			case MMAP_FLAGS:
				log_MMAP_FLAGS(args[i]);
				break;
			case MEM_PROT:
				log_MEM_PROT(args[i]);
				break;
			case ACCESS_MODE:
				log_ACCESS_MODE(args[i]);
				break;
			default:
				printf("%lld", args[i]);
				break;
		}
	}
	long ret = regs->rax;
	// Print return value for pointer-returning syscalls as hex or special
	if (strcmp(name, "brk") == 0 || strcmp(name, "mmap") == 0 || strcmp(name, "mmap2") == 0) {
		if (ret < 0) {
			int err = -ret;
			printf(") = -1 (%s)\n", strerror(err));
		} else {
			printf(") = ");
			print_ptr_special((unsigned long long)ret);
			printf("\n");
		}
	} else if (ret < 0) {
		int err = -ret;
		printf(") = -1 (%s)\n", strerror(err));
	} else {
		printf(") = %ld\n", ret);
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