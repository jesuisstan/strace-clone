#include <ft_strace_utils.h>
#include <execution.h>
#include <analysis.h>
#include <statistics.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ptrace.h>

// External variable to reset execve printing flag
extern bool execve_printed;

/**
 * @brief Execute a program
 *
 * @param config
 * @param statistics
 * @return the exit status of the child process or -1 if an error occurred
 */
int exec_program(const config_t *config, struct s_statistics *statistics)
{
	if (!config || config->argc == 0) {
		fprintf(stderr, "No program specified\n");
		return -1;
	}
	
	// Reset execve printing flag for new process
	execve_printed = false;
	
	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		return -1;
	}
	
	if (pid == 0) {
		// Child process
		if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
			perror("ptrace TRACEME");
			exit(1);
		}
		
		// Raise SIGSTOP to let parent set up tracing
		raise(SIGSTOP);
		
		// execvp сам ищет команду в PATH, поэтому используем config->argv напрямую
		execvp(config->argv[0], config->argv);
		perror("execvp");
		exit(126);
	}
	
	// Parent process
	// Wait for child to stop
	int status;
	if (waitpid(pid, &status, 0) == -1) {
		perror("waitpid");
		return -1;
	}
	
	if (!WIFSTOPPED(status) || WSTOPSIG(status) != SIGSTOP) {
		fprintf(stderr, "Child did not stop as expected\n");
		return -1;
	}
	
	// Set up syscall tracing
	if (ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD) == -1) {
		perror("ptrace SETOPTIONS");
		return -1;
	}
	
	// Continue the child
	if (ptrace(PTRACE_SYSCALL, pid, NULL, NULL) == -1) {
		perror("ptrace SYSCALL");
		return -1;
	}

	status = analysis_routine(pid, statistics);
	
	if (WIFSIGNALED(status)) {
		raise(WTERMSIG(status));
	}
	
	return WEXITSTATUS(status);
}