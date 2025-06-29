#pragma once

#include <sys/types.h>

/**
 * @brief A custom handle for 'clone' syscall since it act weird
 *
 * @param pid the pid of the tracee
 * @param regs the registers
 * @param regs_type the registers types
 * @return int the size written
 */
int custom_handler_clone(pid_t pid, user_regs_t *regs, register_type_t regs_type);