#include "syscall_32.h"
#include "syscall_64.h"
#include "syscall_strace.h"
#include <stdio.h>
#include <string.h>

extern const size_t x86_64_syscalls_count;
const size_t i386_syscalls_count = sizeof(i386_syscalls) / sizeof(i386_syscalls[0]);

static syscall_description_t *get_default_syscall(uint64_t syscall_no) {
    static char default_syscall_name[32] = {0};
    static syscall_description_t default_syscall = {default_syscall_name, INTEGER, {NONE}, NULL};
    snprintf(default_syscall_name, sizeof(default_syscall_name), "unknown_%lu", (unsigned long)syscall_no);
    return &default_syscall;
}

const syscall_description_t *syscall_get_description(uint64_t syscall_no, architecture_t type) {
    const syscall_description_t *selected_syscall;
    if (type == X86_64) {
        if (syscall_no >= x86_64_syscalls_count)
            return get_default_syscall(syscall_no);
        selected_syscall = &x86_64_syscalls[syscall_no];
        if (selected_syscall->name == NULL)
            return get_default_syscall(syscall_no);
    } else {
        if (syscall_no >= sizeof(i386_syscalls)/sizeof(i386_syscalls[0]))
            return get_default_syscall(syscall_no);
        selected_syscall = &i386_syscalls[syscall_no];
        if (selected_syscall->name == NULL)
            return get_default_syscall(syscall_no);
    }
    return selected_syscall;
} 