#include <syscall_strace.h>
#include <ft_strace_utils.h>
#include <statistics.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>

// Global variable to track current syscall number
int current_syscall_no = -1;
bool execve_printed = false;
static int last_execve_ret = -1;
static unsigned long execve_filename_addr = 0;

/**
 * @brief Read a string from traced process memory safely
 *
 * @param pid
 * @param addr
 * @return char* allocated string or NULL if error
 */
static char *read_string_safe(pid_t pid, unsigned long addr)
{
	if (addr == 0) {
		return strdup("(null)");
	}
	
	char *str = malloc(4096);
	if (!str) {
		return NULL;
	}
	
	memset(str, 0, 4096);
	
	// Read data word by word
	for (int i = 0; i < 1024; i++) {
		unsigned long word = ptrace(PTRACE_PEEKDATA, pid, addr + i * 8, NULL);
		if ((long)word == -1 && errno) {
			free(str);
			return strdup("(error)");
		}
		
		// Copy bytes from word
		for (int j = 0; j < 8; j++) {
			char byte = (word >> (j * 8)) & 0xFF;
			str[i * 8 + j] = byte;
			if (byte == '\0') {
				return str;
			}
		}
	}
	
	// Truncate if too long
	str[4095] = '\0';
	return str;
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
		execve_filename_addr = regs->rdi;
		return;
	}
	
	// Handle execve on exit
	if (is_exit && syscall_no == 59) { // execve
		long ret = regs->rax;
		last_execve_ret = (int)ret;
		
		// Only print if this execve succeeded (ret == 0)
		if (ret == 0) {
			// Read filename from saved address
			char *filename = read_string_safe(pid, execve_filename_addr);
			if (!filename) {
				filename = strdup("(error)");
			}
			printf("execve(\"%s\", ...) = 0\n", filename);
			free(filename);
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
			printf("%d, \"%s\", %llu", 
				(int)regs->rdi, 
				read_string_safe(pid, regs->rsi), 
				regs->rdx);
			break;
		case 0: // read
			printf("%d, %p, %llu", 
				(int)regs->rdi, 
				(void*)regs->rsi, 
				regs->rdx);
			break;
		case 257: // openat
			printf("%d, \"%s\", %s, %d", 
				(int)regs->rdi, 
				read_string_safe(pid, regs->rsi), 
				format_open_flags((int)regs->rdx),
				(int)regs->r10);
			break;
		case 21: // access
			printf("\"%s\", %d", 
				read_string_safe(pid, regs->rdi), 
				(int)regs->rsi);
			break;
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
			printf("%d, %p, %llu, %lld", 
				(int)regs->rdi, 
				(void*)regs->rsi, 
				regs->rdx, 
				(long long)regs->r10);
			break;
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
			printf("%p, %llu, %d", 
				(void*)regs->rdi, 
				regs->rsi, 
				(int)regs->rdx);
			break;
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
			printf(" = %ld", ret);
		}
	}
	printf("\n");
}