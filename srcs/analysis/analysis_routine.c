#include <analysis.h>
#include <ft_strace_utils.h>
#include <syscall_strace.h>
#include <statistics.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <signal.h>
#include <unistd.h>
#include <sys/uio.h>

// Define NT_PRSTATUS if not defined
#ifndef NT_PRSTATUS
#define NT_PRSTATUS 1
#endif

/**
 * @brief Main analysis routine
 *
 * @param pid
 * @param statistics
 * @return int
 */
int analysis_routine(pid_t pid, struct s_statistics *statistics __attribute__((unused)))
{
	int status;
	bool is_syscall_entry = true;
	
	while (1) {
		// Wait for the child to stop
		if (waitpid(pid, &status, 0) == -1) {
			perror("waitpid");
			return -1;
		}
		
		// Check if child exited
		if (WIFEXITED(status)) {
			printf("+++ exited with %d +++\n", WEXITSTATUS(status));
			return status;
		}
		
		// Check if child was killed by signal
		if (WIFSIGNALED(status)) {
			printf("+++ killed by %s +++\n", ft_signalname(WTERMSIG(status)));
			return status;
		}
		
		// Check if child stopped
		if (WIFSTOPPED(status)) {
			// Check if it's a syscall stop
			if (WSTOPSIG(status) == (SIGTRAP | 0x80)) {
				// Get registers using PTRACE_GETREGSET
				struct user_regs_struct regs;
				struct iovec iov = {
					.iov_base = &regs,
					.iov_len = sizeof(regs)
				};
				
				if (ptrace(PTRACE_GETREGSET, pid, NT_PRSTATUS, &iov) == -1) {
					perror("ptrace GETREGSET");
					return -1;
				}
				
				// Handle syscall
				syscall_handle(pid, &regs, !is_syscall_entry);
				
				// Toggle entry/exit flag
				is_syscall_entry = !is_syscall_entry;
			}
		}
		
		// Continue the child
		if (ptrace(PTRACE_SYSCALL, pid, NULL, NULL) == -1) {
			perror("ptrace SYSCALL");
			return -1;
		}
	}
}