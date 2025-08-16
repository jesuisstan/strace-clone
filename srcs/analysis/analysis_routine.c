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
#include <string.h>

// Define NT_PRSTATUS if not defined
#ifndef NT_PRSTATUS
#define NT_PRSTATUS 1
#endif

/**
 * @brief Setup signal blocking before waitpid
 * 
 * Unblocks all signals before waitpid to ensure we can receive
 * signals during the wait operation. This prevents deadlocks
 * and allows proper signal handling during process tracing.
 */
static void setup_signals_before_wait(void)
{
	sigset_t empty;
	sigemptyset(&empty);
	sigprocmask(SIG_SETMASK, &empty, NULL);
}

/**
 * @brief Block signals after waitpid
 * 
 * Blocks critical signals (SIGHUP, SIGQUIT, SIGPIPE, SIGTERM) after waitpid
 * to prevent interference with the tracing process. This ensures
 * stable tracing and prevents unexpected termination of ft_strace.
 */
static void block_signals_after_wait(void)
{
	sigset_t blocked;
	sigemptyset(&blocked);
	sigaddset(&blocked, SIGHUP);
	sigaddset(&blocked, SIGQUIT);
	sigaddset(&blocked, SIGPIPE);
	sigaddset(&blocked, SIGTERM);
	sigprocmask(SIG_BLOCK, &blocked, NULL);
}

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
	int last_syscall_no = -1;
	architecture_t current_arch = X86_64; // Default to 64-bit
	extern bool execve_success;
	extern failed_execve_t last_failed_execve;
	extern void print_execve(const char *filename, unsigned long envp_ptr, int arg_count, int env_count, long ret, int err);
	
	// Wait for the child to stop after PTRACE_INTERRUPT
	if (waitpid(pid, &status, 0) == -1) {
		perror("waitpid after PTRACE_INTERRUPT");
		return -1;
	}
	
	// Set ptrace options
	if (ptrace(PTRACE_SETOPTIONS, pid, 0, 0) == -1) {
		perror("ptrace SETOPTIONS");
		return -1;
	}
	// Continue child after initial stop
	if (ptrace(PTRACE_SYSCALL, pid, NULL, NULL) == -1) {
		perror("ptrace initial SYSCALL");
		return -1;
	}

	while (1) {
		// Setup signals before waitpid
		setup_signals_before_wait();
		
		// Wait for the child to stop
		if (waitpid(pid, &status, 0) == -1) {
			perror("waitpid");
			return -1;
		}
		
		// Block signals after waitpid
		block_signals_after_wait();
		
		// Try to get registers even if process might have exited
		// This allows us to see exit_group syscalls
		struct user_regs_struct regs;
		struct iovec iov = {
			.iov_base = &regs,
			.iov_len = sizeof(regs)
		};
		
		// Determine architecture based on register size
		architecture_t detected_arch = (iov.iov_len == sizeof(struct user_regs_struct)) ? X86_64 : X86_I386;
		if (detected_arch != current_arch) {
			current_arch = detected_arch;
			fprintf(stderr, "[ Process PID=%d runs in %d bit mode. ]\n", pid, (detected_arch == X86_64) ? 64 : 32);
			fflush(stderr);
		}
		
		// Check if child stopped
		if (WIFSTOPPED(status)) {
			// Check if it's a syscall stop
			if (WSTOPSIG(status) == SIGTRAP) {
				// Get registers using PTRACE_GETREGSET
				if (ptrace(PTRACE_GETREGSET, pid, NT_PRSTATUS, &iov) == -1) {
					perror("ptrace GETREGSET");
					return -1;
				}
				// Handle syscall - only show syscalls after successful execve
				if (execve_success || (int)regs.orig_rax == 59) { // 59 is execve
					syscall_handle(pid, &regs, !is_syscall_entry);
				}
				
				// Save last syscall number
				last_syscall_no = (int)regs.orig_rax;
				
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
			else
			{
				int signal = WSTOPSIG(status);
				int signal_to_deliver = 0;

				if (signal != SIGTRAP) {
					siginfo_t siginfo;
					if (ptrace(PTRACE_GETSIGINFO, pid, NULL, &siginfo) == -1) {
						perror("ptrace PTRACE_GETSIGINFO");
					} else {
						const char *signame = ft_signalname(siginfo.si_signo);
						const char *sicodename = ft_sicodename(siginfo.si_signo, siginfo.si_code);
						// For memory-related signals, print si_addr instead of si_pid/si_uid
						if (siginfo.si_signo == SIGSEGV || siginfo.si_signo == SIGBUS) {
							fprintf(stderr, "--- %s {si_signo=%s, si_code=%s, si_addr=%p",
									signame, signame, sicodename, siginfo.si_addr);
						} else {
							fprintf(stderr, "--- %s {si_signo=%s, si_code=%s, si_pid=%d, si_uid=%d",
									signame, signame, sicodename, siginfo.si_pid, siginfo.si_uid);
							
							// Add additional fields for SIGCHLD
							if (siginfo.si_signo == SIGCHLD) {
								fprintf(stderr, ", si_status=%d, si_utime=%ld, si_stime=%ld",
										siginfo.si_status, siginfo.si_utime, siginfo.si_stime);
							}
						}
						
						fprintf(stderr, "} ---\n");
						fflush(stderr);
					}
					signal_to_deliver = signal;
				}
				if (ptrace(PTRACE_SYSCALL, pid, NULL, signal_to_deliver) == -1) {
					perror("ptrace SYSCALL with signal");
					return -1;
				}
				continue;
			}
		}
		// Check if child exited - but first try to handle exit_group syscall
		else if (WIFEXITED(status)) {
			// Check if the last syscall was exit_group
			if (last_syscall_no == 231) { // exit_group
				// Print exit_group syscall manually
				fprintf(stderr, "exit_group(%d) = ?\n", WEXITSTATUS(status));
				fflush(stderr);
			}
			// If there was no successful execve, print the last failed one
			if (!execve_success && last_failed_execve.valid && last_failed_execve.filename) {
				print_execve(last_failed_execve.filename, last_failed_execve.envp_ptr, last_failed_execve.arg_count, last_failed_execve.env_count, -1, last_failed_execve.err);
				free(last_failed_execve.filename); last_failed_execve.filename = NULL;
				last_failed_execve.valid = false;
			}
			fprintf(stderr, "+++ exited with %d +++\n", WEXITSTATUS(status));
			fflush(stderr);
			return status;
		}
		// Check if child was killed by signal (after processing syscall)
		else if (WIFSIGNALED(status)) {
			// If there was no successful execve, print the last failed one
			if (!execve_success && last_failed_execve.valid && last_failed_execve.filename) {
				print_execve(last_failed_execve.filename, last_failed_execve.envp_ptr, last_failed_execve.arg_count, last_failed_execve.env_count, -1, last_failed_execve.err);
				free(last_failed_execve.filename); last_failed_execve.filename = NULL;
				last_failed_execve.valid = false;
			}
			fprintf(stderr, "+++ killed by %s +++\n", strsignal(WTERMSIG(status)));
			fflush(stderr);
			return status;
		}
		// Continue the child
		if (ptrace(PTRACE_SYSCALL, pid, NULL, NULL) == -1) {
			perror("ptrace SYSCALL");
			return -1;
		}
	}
	
	return 0;
}