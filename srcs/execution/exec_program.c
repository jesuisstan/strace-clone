#include <ft_strace_utils.h>
#include <execution.h>
#include <analysis.h>
#include <statistics.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ptrace.h>

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
		// execvp сам ищет команду в PATH, поэтому используем config->argv напрямую
		execvp(config->argv[0], config->argv);
		perror("execvp");
		exit(126);
	}
	
	// Parent process
	// setup_tracing(pid) больше не нужен

	int status = analysis_routine(pid, statistics);
	
	if (WIFSIGNALED(status)) {
		raise(WTERMSIG(status));
	}
	
	return WEXITSTATUS(status);
}