#pragma once

#include <syscall_strace.h>

extern const syscall_description_t x86_64_syscalls[];

const char *syscall_get_description(int syscall_no);