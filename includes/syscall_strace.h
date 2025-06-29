#ifndef SYSCALL_STRACE_H
#define SYSCALL_STRACE_H

#include <sys/types.h>
#include <sys/user.h>
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
	ACCESS_MODE
	// Add more types as needed
} param_type_t;

typedef struct {
	const char *name;
	param_type_t param_types[6];
} syscall_description_t;

extern const syscall_description_t x86_64_syscalls[];

const char *syscall_get_description(int syscall_no);
void syscall_handle(pid_t pid, struct user_regs_struct *regs, bool is_exit);

#endif