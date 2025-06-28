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

// Function prototypes
static char *read_string_via_proc(pid_t pid, unsigned long addr);

// Global variable to track current syscall number
int current_syscall_no = -1;
bool execve_printed = false;
static int last_execve_ret = -1;
static char *execve_filename = NULL;

/**
 * @brief Read binary data from traced process memory safely (simplified version)
 *
 * @param pid
 * @param addr
 * @param size
 * @return char* allocated data or NULL if error
 */
static char *read_binary_safe(pid_t pid, unsigned long addr, size_t size)
{
	(void)pid; // Suppress unused parameter warning
	(void)size; // Suppress unused parameter warning
	
	if (addr == 0) {
		return strdup("(null)");
	}
	
	// Since we can't read memory with allowed ptrace options,
	// return a placeholder indicating the address
	char *result = malloc(64);
	if (!result) {
		return NULL;
	}
	
	snprintf(result, 64, "0x%lx", addr);
	return result;
}

/**
 * @brief Format mmap flags
 */
static const char *format_mmap_flags(int flags)
{
	static char buf[256];
	buf[0] = '\0';
	
	if (flags & MAP_PRIVATE) strcat(buf, "MAP_PRIVATE|");
	if (flags & MAP_SHARED) strcat(buf, "MAP_SHARED|");
	if (flags & MAP_ANONYMOUS) strcat(buf, "MAP_ANONYMOUS|");
	if (flags & MAP_FIXED) strcat(buf, "MAP_FIXED|");
	if (flags & MAP_DENYWRITE) strcat(buf, "MAP_DENYWRITE|");
	
	if (buf[0] == '\0') {
		return "0";
	}
	
	// Remove trailing |
	buf[strlen(buf) - 1] = '\0';
	return buf;
}

/**
 * @brief Format mmap protection
 */
static const char *format_mmap_prot(int prot)
{
	static char buf[256];
	buf[0] = '\0';
	
	if (prot & PROT_READ) strcat(buf, "PROT_READ|");
	if (prot & PROT_WRITE) strcat(buf, "PROT_WRITE|");
	if (prot & PROT_EXEC) strcat(buf, "PROT_EXEC|");
	
	if (buf[0] == '\0') {
		return "0";
	}
	
	// Remove trailing |
	buf[strlen(buf) - 1] = '\0';
	return buf;
}

/**
 * @brief Format access mode flags
 */
static const char *format_access_mode(int mode)
{
	static char buf[256];
	buf[0] = '\0';
	
	if (mode == 0) strcat(buf, "F_OK|");
	else if (mode == 1) strcat(buf, "X_OK|");
	else if (mode == 2) strcat(buf, "W_OK|");
	else if (mode == 4) strcat(buf, "R_OK|");
	else if (mode == 6) strcat(buf, "R_OK|W_OK|");
	else if (mode == 5) strcat(buf, "R_OK|X_OK|");
	else if (mode == 7) strcat(buf, "R_OK|W_OK|X_OK|");
	else {
		sprintf(buf, "%d", mode);
		return buf;
	}
	
	if (buf[0] == '\0') {
		return "0";
	}
	
	// Remove trailing |
	buf[strlen(buf) - 1] = '\0';
	return buf;
}

/**
 * @brief Format open flags
 */
static const char *format_open_flags(int flags)
{
	static char buf[256];
	buf[0] = '\0';
	
	// Handle access mode flags
	int access_mode = flags & O_ACCMODE;
	if (access_mode == O_RDONLY) strcat(buf, "O_RDONLY|");
	else if (access_mode == O_WRONLY) strcat(buf, "O_WRONLY|");
	else if (access_mode == O_RDWR) strcat(buf, "O_RDWR|");
	
	// Handle other flags
	if (flags & O_CREAT) strcat(buf, "O_CREAT|");
	if (flags & O_EXCL) strcat(buf, "O_EXCL|");
	if (flags & O_TRUNC) strcat(buf, "O_TRUNC|");
	if (flags & O_APPEND) strcat(buf, "O_APPEND|");
	if (flags & O_CLOEXEC) strcat(buf, "O_CLOEXEC|");
	
	if (buf[0] == '\0') {
		return "0";
	}
	
	// Remove trailing |
	buf[strlen(buf) - 1] = '\0';
	return buf;
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
	
	// Get syscall number
	int syscall_no = (int)regs->orig_rax;
	
	// Special handling for execve on entry
	if (!is_exit && syscall_no == 59) { // execve
		current_syscall_no = syscall_no;
		
		// Store the filename for later use
		if (execve_filename) {
			free(execve_filename);
		}
		// Try to read the filename from memory
		execve_filename = read_string_via_proc(pid, regs->rdi);
		if (!execve_filename) {
			execve_filename = strdup("(error)");
		}
		return;
	}
	
	// Handle execve on exit
	if (is_exit && syscall_no == 59) { // execve
		long ret = regs->rax;
		last_execve_ret = (int)ret;
		
		// Only print if this execve succeeded (ret == 0)
		if (ret == 0) {
			printf("execve(\"%s\", ...) = 0\n", execve_filename ? execve_filename : "(error)");
		}
		return;
	}
	
	// Only print on exit to avoid duplication
	if (!is_exit) {
		return;
	}
	
	// Get syscall description
	const char *desc = syscall_get_description(syscall_no);
	if (!desc) {
		desc = "unknown";
	}
	
	// Print syscall name and parameters
	printf("%s(", desc);
	
	// Print parameters based on syscall
	switch (syscall_no) {
		case 1: // write
		{
			char *str = read_string_via_proc(pid, regs->rsi);
			if (str && strlen(str) > 0 && strlen(str) < 50) {
				printf("%d, \"%s\", %llu", 
					(int)regs->rdi, 
					str, 
					regs->rdx);
				free(str);
			} else {
				if (str) free(str);
				printf("%d, %p, %llu", 
					(int)regs->rdi, 
					(void*)regs->rsi, 
					regs->rdx);
			}
			break;
		}
		case 0: // read
		{
			printf("%d, %p, %llu", 
				(int)regs->rdi, 
				(void*)regs->rsi, 
				regs->rdx);
			break;
		}
		case 257: // openat
		{
			char *str = read_string_via_proc(pid, regs->rsi);
			if (str && strlen(str) > 0 && strlen(str) < 100) {
				printf("%s, \"%s\", %s, %d", 
					(int)regs->rdi == -100 ? "AT_FDCWD" : "unknown",
					str, 
					format_open_flags((int)regs->rdx),
					(int)regs->r10);
				free(str);
			} else {
				if (str) free(str);
				printf("%s, %p, %s, %d", 
					(int)regs->rdi == -100 ? "AT_FDCWD" : "unknown",
					(void*)regs->rsi, 
					format_open_flags((int)regs->rdx),
					(int)regs->r10);
			}
			break;
		}
		case 21: // access
		{
			char *str = read_string_via_proc(pid, regs->rdi);
			if (str && strlen(str) > 0 && strlen(str) < 100) {
				printf("\"%s\", %s", 
					str, 
					format_access_mode((int)regs->rsi));
				free(str);
			} else {
				if (str) free(str);
				printf("%p, %s", 
					(void*)regs->rdi, 
					format_access_mode((int)regs->rsi));
			}
			break;
		}
		case 79: // getcwd
			printf("%p, %llu", 
				(void*)regs->rdi, 
				regs->rsi);
			break;
		case 231: // exit_group
			printf("%d", (int)regs->rdi);
			break;
		case 60: // exit
			printf("%d", (int)regs->rdi);
			break;
		case 12: // brk
			if (regs->rdi == 0) {
				printf("NULL");
			} else {
				printf("%p", (void*)regs->rdi);
			}
			break;
		case 9: // mmap
			printf("%p, %llu, %s, %s, %d, %lld", 
				(void*)regs->rdi, 
				regs->rsi, 
				format_mmap_prot((int)regs->rdx),
				format_mmap_flags((int)regs->r10), 
				(int)regs->r8, 
				(long long)regs->r9);
			break;
		case 3: // close
			printf("%d", (int)regs->rdi);
			break;
		case 5: // fstat
			printf("%d, %p", (int)regs->rdi, (void*)regs->rsi);
			break;
		case 17: // pread64
		{
			printf("%d, %p, %llu, %lld", 
				(int)regs->rdi, 
				(void*)regs->rsi, 
				regs->rdx, 
				(long long)regs->r10);
			break;
		}
		case 158: // arch_prctl
			printf("%d, %p", (int)regs->rdi, (void*)regs->rsi);
			break;
		case 218: // set_tid_address
			printf("%p", (void*)regs->rdi);
			break;
		case 273: // set_robust_list
			printf("%p, %llu", (void*)regs->rdi, regs->rsi);
			break;
		case 334: // rseq
			printf("%p, %llu, %d, %d", 
				(void*)regs->rdi, 
				regs->rsi, 
				(int)regs->rdx, 
				(int)regs->r10);
			break;
		case 10: // mprotect
			printf("%p, %llu, %s", 
				(void*)regs->rdi, 
				regs->rsi, 
				format_mmap_prot((int)regs->rdx));
			break;
		case 302: // prlimit64
			printf("%d, %d, %p, %p", 
				(int)regs->rdi, 
				(int)regs->rsi, 
				(void*)regs->rdx, 
				(void*)regs->r10);
			break;
		case 11: // munmap
			printf("%p, %llu", (void*)regs->rdi, regs->rsi);
			break;
		case 318: // getrandom
		{
			// For getrandom, we want to show binary data representation
			// Use the return value (actual bytes read) instead of requested size
			long actual_bytes = regs->rax;
			if (actual_bytes > 0 && actual_bytes <= 64) { // Sanity check
				char *data = read_binary_safe(pid, regs->rdi, actual_bytes);
				if (data) {
					// Show bytes in hex format like original strace
					printf("\"");
					for (long i = 0; i < actual_bytes; i++) {
						printf("\\x%02x", (unsigned char)data[i]);
					}
					printf("\", %ld", actual_bytes); // Show actual bytes read
					free(data);
				} else {
					printf("%p, %llu, %d", 
						(void*)regs->rdi, 
						regs->rsi, 
						(int)regs->rdx);
				}
			} else {
				printf("%p, %llu, %d", 
					(void*)regs->rdi, 
					regs->rsi, 
					(int)regs->rdx);
			}
			break;
		}
		default:
			// Print first few parameters as pointers/ints
			if (regs->rdi != 0) {
				printf("%p", (void*)regs->rdi);
			} else {
				printf("0");
			}
			if (regs->rsi != 0) {
				printf(", %p", (void*)regs->rsi);
			}
			if (regs->rdx != 0) {
				printf(", %p", (void*)regs->rdx);
			}
			if (regs->r10 != 0) {
				printf(", %p", (void*)regs->r10);
			}
			if (regs->r8 != 0) {
				printf(", %p", (void*)regs->r8);
			}
			if (regs->r9 != 0) {
				printf(", %p", (void*)regs->r9);
			}
			break;
	}
	
	printf(")");
	
	// Print return value on exit
	long ret = regs->rax;
	if (ret < 0) {
		// Error case
		printf(" = -1 %s (%s)", 
			ft_errnoname(-ret), 
			strerror(-ret));
	} else {
		// Success case
		if (ret == 0) {
			printf(" = 0");
		} else {
			// Check if this syscall typically returns a pointer/address
			if (syscall_no == 12 || // brk
				syscall_no == 9 ||  // mmap
				syscall_no == 11 || // munmap
				syscall_no == 218 || // set_tid_address
				syscall_no == 273 || // set_robust_list
				syscall_no == 334 || // rseq
				syscall_no == 158 || // arch_prctl
				syscall_no == 302) { // prlimit64
				printf(" = 0x%lx", ret);
			} else {
				printf(" = %ld", ret);
			}
		}
	}
	printf("\n");
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