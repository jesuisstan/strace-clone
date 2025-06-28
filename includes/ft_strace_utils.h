#ifndef FT_STRACE_UTILS_H
# define FT_STRACE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

// Replace ft_printf functions with standard ones
#define ft_printf printf
#define ft_dprintf dprintf
#define ft_snprintf snprintf
#define ft_asprintf asprintf

// Replace ft_string functions with standard ones
#define ft_strlen strlen
#define ft_strdup strdup
#define ft_strchr strchr
#define ft_strtok strtok
#define ft_strcmp strcmp
#define ft_strncmp strncmp
#define ft_memcpy memcpy
#define ft_memmove memmove
#define ft_memcmp memcmp
#define ft_atoi atoi

// Replace ft_test functions
#define ft_test_assert assert

// Custom functions that need implementation
const char *ft_signalname(int sig);
const char *ft_errnoname(int errnum);
const char *ft_strerror(int errnum);
char *ft_escape(const char *str, size_t size);

#endif