#ifndef SYSCALL_STRACE_H
#define SYSCALL_STRACE_H

#include <sys/types.h>
#include <sys/user.h>
#include <types.h>

// Forward declarations
struct s_statistics;

// Global variable to track current syscall number
extern unsigned long long current_syscall_no;

typedef enum {
	NONE = 0,
	INTEGER,
	STRING,
	POINTER,
	HEX
} param_type_t;

typedef struct {
	const char *name;
	param_type_t param_types[6];
} syscall_description_t;

const syscall_description_t *syscall_get_description(unsigned long long syscall_no, architecture_t arch);
void syscall_handle(pid_t pid, struct user_regs_struct *regs, struct s_statistics *statistics, int is_syscall_entry);

#endif