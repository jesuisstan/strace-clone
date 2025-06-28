#include <ft_strace_utils.h>
#include <syscall_strace.h>
#include <execution.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * @brief Search for a command in path
 *
 * @param command_name The command to search in path
 * @return char* The path to the command, or NULL if not found
 */
char *search_in_path(const char *command_name)
{
	if (!command_name)
		return NULL;
	
	// If command contains '/', it's a path
	if (strchr(command_name, '/'))
		return strdup(command_name);
	
	const char *path = getenv("PATH");
	if (!path)
		return NULL;
	
	char *path_copy = strdup(path);
	if (!path_copy)
		return NULL;
	
	char *path_token = strtok(path_copy, ":");
	while (path_token) {
		size_t path_len = strlen(path_token);
		size_t cmd_len = strlen(command_name);
		char *command_path_to_test = malloc(path_len + cmd_len + 2); // +2 for '/' and '\0'
		
		if (!command_path_to_test) {
			free(path_copy);
			return NULL;
		}
		
		strcpy(command_path_to_test, path_token);
		command_path_to_test[path_len] = '/';
		strcpy(command_path_to_test + path_len + 1, command_name);
		
		if (access(command_path_to_test, X_OK) == 0) {
			free(path_copy);
			return command_path_to_test;
		}
		
		free(command_path_to_test);
		path_token = strtok(NULL, ":");
	}
	
	free(path_copy);
	return NULL;
}