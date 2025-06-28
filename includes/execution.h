#ifndef EXECUTION_H
# define EXECUTION_H

#include <config.h>
#include <types.h>
#include <sys/types.h>

// Forward declarations
struct s_statistics;

#define EXEC_ERROR -1
#define EXEC_CHILD_END -2

/**
 * @brief Execute a program
 *
 * @param config
 * @param statistics
 * @return the pid of the child process or EXEC_ERROR if an error occurred
 */
int exec_program(const config_t *config, struct s_statistics *statistics);

/**
 * @brief Setup tracing for tracee
 *
 * @param pid the pid of the tracee
 * @return 1 if an error occurred, 0 otherwise
 */
int setup_tracing(pid_t pid);

/**
 * @brief Search for a command in path
 *
 * @param command_name The command to search in path
 * @param path The path to search in
 * @return char* The path to the command, or NULL if not found
 */
char *search_in_path(const char *command_name);

#endif