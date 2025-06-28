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

const char *syscall_get_description(int syscall_no);
void syscall_handle(pid_t pid, struct user_regs_struct *regs, bool is_exit);

#endif