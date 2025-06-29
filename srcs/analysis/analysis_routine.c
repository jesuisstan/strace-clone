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
#include <sys/time.h>
#include <time.h>

// Define NT_PRSTATUS if not defined
#ifndef NT_PRSTATUS
#define NT_PRSTATUS 1
#endif

/**
 * @brief Get current time in microseconds with high precision
 */
static unsigned long long get_time_us(void)
{
	struct timespec ts;
	if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
		return (unsigned long long)ts.tv_sec * 1000000ULL + (unsigned long long)ts.tv_nsec / 1000ULL;
	}
	// Fallback to gettimeofday if clock_gettime fails
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (unsigned long long)tv.tv_sec * 1000000ULL + (unsigned long long)tv.tv_usec;
}

/**
 * @brief Main analysis routine
 *
 * @param pid
 * @param statistics
 * @return int
 */
int analysis_routine(pid_t pid, struct s_statistics *statistics)
{
	int status;
	bool is_syscall_entry = true;
	unsigned long long syscall_start_time = 0;
	extern bool execve_success;
	extern failed_execve_t last_failed_execve;
	extern void print_execve(pid_t pid, const char *filename, unsigned long argv_ptr, unsigned long envp_ptr, int arg_count, int env_count, long ret, int err);
	
	// Wait for the child to stop (already done in exec_program)
	// Устанавливаем опции для ptrace, чтобы получать SIGTRAP|0x80 на syscalls
	if (ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD) == -1) {
		perror("ptrace SETOPTIONS");
		return -1;
	}
	// Continue child after initial SIGSTOP
	if (ptrace(PTRACE_SYSCALL, pid, NULL, NULL) == -1) {
		perror("ptrace initial SYSCALL");
		return -1;
	}

	while (1) {
		// Wait for the child to stop
		if (waitpid(pid, &status, 0) == -1) {
			perror("waitpid");
			return -1;
		}
		// Check if child exited
		if (WIFEXITED(status)) {
			// Если не было успешного execve, вывести последний неудачный
			if (!execve_success && last_failed_execve.valid && last_failed_execve.filename) {
				print_execve(pid, last_failed_execve.filename, last_failed_execve.argv_ptr, last_failed_execve.envp_ptr, last_failed_execve.arg_count, last_failed_execve.env_count, -1, last_failed_execve.err);
				free(last_failed_execve.filename); last_failed_execve.filename = NULL;
				last_failed_execve.valid = false;
			}
			fprintf(stderr, "+++ exited with %d +++\n", WEXITSTATUS(status));
			fflush(stderr);
			return status;
		}
		// Check if child was killed by signal
		if (WIFSIGNALED(status)) {
			// Если не было успешного execve, вывести последний неудачный
			if (!execve_success && last_failed_execve.valid && last_failed_execve.filename) {
				print_execve(pid, last_failed_execve.filename, last_failed_execve.argv_ptr, last_failed_execve.envp_ptr, last_failed_execve.arg_count, last_failed_execve.env_count, -1, last_failed_execve.err);
				free(last_failed_execve.filename); last_failed_execve.filename = NULL;
				last_failed_execve.valid = false;
			}
			fprintf(stderr, "+++ killed by %s +++\n", strsignal(WTERMSIG(status)));
			fflush(stderr);
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
				
				// Add to statistics if enabled
				if (statistics && !is_syscall_entry) {
					// Only count on exit to avoid double counting
					int syscall_no = (int)regs.orig_rax;
					unsigned long long duration = get_time_us() - syscall_start_time;
					long ret = regs.rax;
					
					// Check for errors (negative return values indicate errors)
					bool is_error = (ret < 0);
					
					// Special handling for execve - only count once when it succeeds
					if (syscall_no == 59) { // execve
						if (ret == 0) {
							// Only count successful execve once
							statistics_add_entry(statistics, X86_64, syscall_no, duration);
							// Continue tracing after successful execve
						}
					} else {
						statistics_add_entry(statistics, X86_64, syscall_no, duration);
					}
					
					// Update error count if this was an error
					if (is_error) {
						t_statistics_entry *entry = statistics_get(statistics, X86_64, syscall_no);
						if (entry) {
							entry->error_count++;
						}
					}
				}
				
				// Record start time for next syscall entry
				if (is_syscall_entry && statistics) {
					syscall_start_time = get_time_us();
				}
				
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
	
	return 0;
}