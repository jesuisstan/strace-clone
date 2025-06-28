#include <analysis.h>
#include <ft_strace_utils.h>
#include <syscall_strace.h>
#include <statistics.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <signal.h>
#include <unistd.h>

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
				// Get registers
				struct user_regs_struct regs;
				if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) == -1) {
					perror("ptrace GETREGS");
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