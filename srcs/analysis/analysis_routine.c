#include <analysis.h>
#include <ft_strace_utils.h>
#include <syscall_strace.h>
#include <statistics.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <signal.h>

unsigned long long current_syscall_no = 0;

/**
 * @brief Analysis routine of the tracer
 *
 * @param pid the pid of the tracee
 * @return status code of tracee or ROUTINE_ERROR if an error occurred
 */
int analysis_routine(pid_t pid, struct s_statistics *statistics)
{
	int status;
	int is_syscall_entry = 1; // Track if we're on syscall entry or exit
	
	// Wait for the process to stop after execvp
	if (waitpid(pid, &status, 0) == -1) {
		perror("waitpid");
		return -1;
	}
	
	if (WIFEXITED(status)) {
		dprintf(STDERR_FILENO, "+++ exited with %d +++\n", WEXITSTATUS(status));
		return status;
	}
	
	if (WIFSIGNALED(status)) {
		dprintf(STDERR_FILENO, "+++ killed by %s +++\n", ft_signalname(WTERMSIG(status)));
		return status;
	}
	
	// Start tracing syscalls
	if (ptrace(PTRACE_SYSCALL, pid, NULL, NULL) == -1) {
		perror("ptrace SYSCALL");
		return -1;
	}
	
	while (1) {
		if (waitpid(pid, &status, 0) == -1) {
			perror("waitpid");
			return -1;
		}
		
		if (WIFEXITED(status)) {
			dprintf(STDERR_FILENO, "+++ exited with %d +++\n", WEXITSTATUS(status));
			return status;
		}
		
		if (WIFSIGNALED(status)) {
			dprintf(STDERR_FILENO, "+++ killed by %s +++\n", ft_signalname(WTERMSIG(status)));
			return status;
		}
		
		if (WIFSTOPPED(status)) {
			if (WSTOPSIG(status) == SIGTRAP) {
				// Handle syscall
				struct user_regs_struct regs;
				if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) == -1) {
					perror("ptrace GETREGS");
					return -1;
				}
				
				if (is_syscall_entry) {
					current_syscall_no = regs.orig_rax;
					// On entry: print syscall name and parameters
					syscall_handle(pid, &regs, statistics, 1);
					
					// Special handling for execve - we need to capture it before it replaces the process
					if (current_syscall_no == 59) { // execve
						// Continue to let execve execute, but we won't see the exit
						if (ptrace(PTRACE_SYSCALL, pid, NULL, NULL) == -1) {
							perror("ptrace SYSCALL");
							return -1;
						}
						continue;
					}
				} else {
					// On exit: print return value
					syscall_handle(pid, &regs, statistics, 0);
				}
				
				is_syscall_entry = !is_syscall_entry; // Toggle entry/exit
			}
			
			// Continue to next syscall
			if (ptrace(PTRACE_SYSCALL, pid, NULL, NULL) == -1) {
				perror("ptrace SYSCALL");
				return -1;
			}
		}
	}
}