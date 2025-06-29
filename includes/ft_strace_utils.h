#ifndef FT_STRACE_UTILS_H
# define FT_STRACE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>

// Global variable to track statistics mode
extern bool g_statistics_mode;

// Replace ft_string functions with standard ones
#define ft_memcmp memcmp

// Custom functions that need implementation
const char *ft_signalname(int sig);
const char *ft_errnoname(int errnum);
const char *ft_strerror(int errnum);
char *ft_escape(const char *str, size_t size);
void log_error(const char *context, const char *message, int show_error);

#endif