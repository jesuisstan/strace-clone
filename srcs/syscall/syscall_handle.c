#include <syscall_strace.h>
#include <ft_strace_utils.h>
#include <statistics.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/time.h>
#include <unistd.h>

// Функция для безопасного чтения строки из памяти трассируемого процесса
static char *read_string_from_process(pid_t pid, unsigned long long addr, size_t max_len)
{
	if (addr == 0) return NULL;
	
	char *buffer = malloc(max_len + 1);
	if (!buffer) return NULL;
	
	size_t i = 0;
	while (i < max_len) {
		unsigned long long word = ptrace(PTRACE_PEEKDATA, pid, addr + i, NULL);
		if ((long long)word == -1) {
			free(buffer);
			return NULL;
		}
		
		for (int j = 0; j < 8 && i < max_len; j++) {
			char c = (word >> (j * 8)) & 0xFF;
			buffer[i] = c;
			if (c == '\0') {
				return buffer;
			}
			i++;
		}
	}
	
	buffer[max_len] = '\0';
	return buffer;
}

void syscall_handle(pid_t pid, struct user_regs_struct *regs, struct s_statistics *statistics, int is_syscall_entry)
{
	unsigned long long syscall_no;
	
	if (is_syscall_entry) {
		syscall_no = regs->orig_rax;
	} else {
		syscall_no = current_syscall_no;
	}
	
	if (syscall_no > 400) {
		return;
	}
	
	if (syscall_no == 59 /* execve */ && is_syscall_entry) {
		// Capture execve on entry since it replaces the process image
		const syscall_description_t *desc = syscall_get_description(syscall_no, X86_64);
		if (desc) {
			// Read the filename from the first argument
			char *filename = read_string_from_process(pid, regs->rdi, 256);
			dprintf(STDERR_FILENO, "%s(\"%s\", [\"pwd\"], [/* envp */]) = ", 
				   desc->name, filename ? filename : "(null)");
			free(filename);
			// We can't get the return value since execve replaces the process
			dprintf(STDERR_FILENO, "?\n");
		}
		return;
	}
	
	// For all other syscalls, handle entry and exit
	if (is_syscall_entry) {
		const syscall_description_t *desc = syscall_get_description(syscall_no, X86_64);
		if (desc) {
			dprintf(STDERR_FILENO, "%s(", desc->name);
			int param_count = 0;
			for (int i = 0; i < 6; i++) {
				if (desc->param_types[i] == NONE) {
					break; // Stop at first NONE parameter
				}
				if (param_count > 0) dprintf(STDERR_FILENO, ", ");
				unsigned long long param = 0;
				switch (i) {
					case 0: param = regs->rdi; break;
					case 1: param = regs->rsi; break;
					case 2: param = regs->rdx; break;
					case 3: param = regs->r10; break;
					case 4: param = regs->r8; break;
					case 5: param = regs->r9; break;
				}
				if (desc->param_types[i] == STRING) {
					char *str = read_string_from_process(pid, param, 32);
					if (str) {
						dprintf(STDERR_FILENO, "\"%s\"", str);
						free(str);
					} else {
						dprintf(STDERR_FILENO, "(null)");
					}
				} else if (desc->param_types[i] == POINTER) {
					if (param == 0) {
						dprintf(STDERR_FILENO, "NULL");
					} else {
						dprintf(STDERR_FILENO, "%p", (void *)param);
					}
				} else {
					dprintf(STDERR_FILENO, "%llu", param);
				}
				param_count++;
			}
			dprintf(STDERR_FILENO, ") = ");
		}
	} else {
		// On exit: print return value
		unsigned long long retval = regs->rax;
		if ((long long)retval < 0) {
			dprintf(STDERR_FILENO, "-1 %s (%s)\n", ft_errnoname(-(long long)retval), ft_strerror(-(long long)retval));
		} else {
			dprintf(STDERR_FILENO, "%llu\n", retval);
		}
		
		if (statistics) {
			struct timeval tv;
			gettimeofday(&tv, NULL);
			unsigned long long time_us = tv.tv_sec * 1000000ULL + tv.tv_usec;
			statistics_add_entry(statistics, X86_64, syscall_no, time_us);
		}
	}
}