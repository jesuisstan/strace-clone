#ifndef EXECUTION_H
# define EXECUTION_H

#include <sys/types.h>
#include <config.h>
#include <statistics.h>

// Forward declarations
struct s_statistics;

#define EXEC_ERROR -1
#define EXEC_CHILD_END -2

/**
 * @brief Execute the program to be traced
 *
 * @param config the configuration structure
 * @param statistics optional statistics structure
 * @return int 0 if the call succeeded, -1 otherwise
 */
int exec_program(const t_config *config, t_statistics *statistics);

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