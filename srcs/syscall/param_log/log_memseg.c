#define _GNU_SOURCE

#include "param_log.h"
#include <ft_strace_utils.h>
#include <sys/uio.h>
#include <stdint.h>
#include <sys/user.h>

// Working stubs for register access
static int64_t registers_get_param(void *regs, int type __attribute__((unused)), int index) {
	struct user_regs_struct *user_regs = (struct user_regs_struct *)regs;
	switch (index) {
		case 0: return user_regs->rdi;
		case 1: return user_regs->rsi;
		case 2: return user_regs->rdx;
		case 3: return user_regs->r10;
		case 4: return user_regs->r8;
		case 5: return user_regs->r9;
		default: return 0;
	}
}

static int64_t REGISTERS_GET_RETURN(void *regs, int type __attribute__((unused))) {
	struct user_regs_struct *user_regs = (struct user_regs_struct *)regs;
	return user_regs->rax;
}

#define MAX_PRINT_SIZE 32
#define MAX_DISPLAY_SIZE 30  // Show only 30 chars, then ...

/**
 * @brief Escape string for write syscall (more readable format)
 *
 * @param str the string to escape
 * @param size the size of the string
 * @return char* the escaped string
 */
char *escape_for_write(const char *str, size_t size)
{
	if (!str)
		return NULL;
	
	// Calculate needed size
	size_t needed = 1; // null terminator
	for (size_t i = 0; i < size && str[i]; i++) {
		if (str[i] == '\n')
			needed += 2; // \n format
		else if (str[i] == '\t')
			needed += 2; // \t format
		else if (str[i] == '\r')
			needed += 2; // \r format
		else if (str[i] < 32 || str[i] > 126 || str[i] == '\\' || str[i] == '"')
			needed += 4; // \xXX format
		else
			needed += 1;
	}
	
	char *result = malloc(needed);
	if (!result)
		return NULL;
	
	size_t j = 0;
	for (size_t i = 0; i < size && str[i]; i++) {
		if (str[i] == '\n') {
			result[j++] = '\\';
			result[j++] = 'n';
		} else if (str[i] == '\t') {
			result[j++] = '\\';
			result[j++] = 't';
		} else if (str[i] == '\r') {
			result[j++] = '\\';
			result[j++] = 'r';
		} else if (str[i] < 32 || str[i] > 126 || str[i] == '\\' || str[i] == '"') {
			snprintf(result + j, 5, "\\x%02x", (unsigned char)str[i]);
			j += 4;
		} else {
			result[j++] = str[i];
		}
	}
	result[j] = '\0';
	
	return result;
}

/**
 * @brief Log a memory segment from a remote process
 *
 * @param pid the pid of the remote process
 * @param remote_ptr the pointer to the memory segment in the remote process
 * @param buffer_size the size of the memory segment
 * @return int
 */
int log_memseg_remote(pid_t pid, void *remote_ptr, size_t buffer_size)
{
	size_t to_read = buffer_size > MAX_PRINT_SIZE ? MAX_PRINT_SIZE : buffer_size;
	char *buffer = malloc(to_read);
	if (!buffer)
	{
		log_error("log_MEM", "malloc failed", true);
		return 0;
	}
	if (remote_memcpy(buffer, pid, remote_ptr, to_read) < 0)
	{
		free(buffer);
		return dprintf(STDERR_FILENO, "%p", remote_ptr);
	}
	
	// For display, we want to show only a reasonable number of characters
	size_t display_size = to_read > MAX_DISPLAY_SIZE ? MAX_DISPLAY_SIZE : to_read;
	char *escaped_buffer = escape_for_write(buffer, display_size);
	int size_written;
	if (buffer_size > display_size)
		size_written = dprintf(STDERR_FILENO, "\"%s\"...", escaped_buffer);
	else
		size_written = dprintf(STDERR_FILENO, "\"%s\"", escaped_buffer);
	free(escaped_buffer);
	free(buffer);
	return size_written;
}

/**
 * @brief log memory segment
 *
 * @param value the value
 * @param context the context
 */
int log_MEMSEG(uint64_t value, syscall_log_param_t *context)
{
	if (value == 0)
		return dprintf(STDERR_FILENO, "NULL");
	
	int64_t buffer_size = REGISTERS_GET_RETURN(context->regs, context->type);
	if (buffer_size < 0)
	{
		if (context->after_syscall)
			return dprintf(STDERR_FILENO, "%p", (void *)value);
		buffer_size = registers_get_param(context->regs, context->type, context->arg_index + 1);
	}
	
	return log_memseg_remote(context->pid, (void *)value, buffer_size);
}