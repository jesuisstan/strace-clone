#include <ft_strace_utils.h>
#include "execution.h"
#include "analysis.h"
#include "statistics.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ptrace.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

// External variable to reset execve printing flag
extern bool execve_printed;

/**
 * @brief Execute a program
 *
 * @param config
 * @param statistics
 * @return the exit status of the child process or -1 if an error occurred
 */
int exec_program(const t_config *config, t_statistics *statistics)
{
	if (!config || !config->argv) {
		return -1;
	}
	
	// Find the program to execute (first non-option argument)
	const char *program = NULL;
	char **program_args = NULL;
	
	for (int i = 1; i < config->argc; i++) {
		if (config->argv[i][0] != '-') {
			program = config->argv[i];
			program_args = (char **)&config->argv[i];
			break;
		}
	}
	
	if (!program) {
		fprintf(stderr, "No program specified\n");
		return -1;
	}
	
	// Reset execve printing flag for new process
	execve_printed = false;
	
	// Fork the process
	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		return -1;
	}
	
	if (pid == 0) {
		// Child process - just execute the program
		if (execvp(program, program_args) == -1) {
			perror("execvp");
			exit(1);
		}
	} else {
		// Parent process - attach to child using PTRACE_SEIZE
		if (ptrace(PTRACE_SEIZE, pid, NULL, NULL) == -1) {
			perror("ptrace PTRACE_SEIZE");
			return -1;
		}
		
		// Interrupt the child to start tracing
		if (ptrace(PTRACE_INTERRUPT, pid, NULL, NULL) == -1) {
			perror("ptrace PTRACE_INTERRUPT");
			return -1;
		}
		
		// Start analysis routine
		return analysis_routine(pid, statistics);
	}
	
	return 0;
}