#define _GNU_SOURCE

#include "param_log.h"
#include <sys/prctl.h>
#include <asm/prctl.h>

static const flag_str_t arch_prctl_option_flags[] = {
	FLAG_STR(ARCH_SET_GS),
	FLAG_STR(ARCH_SET_FS),
	FLAG_STR(ARCH_GET_FS),
	FLAG_STR(ARCH_GET_GS),
	FLAG_STR(ARCH_GET_CPUID),
	FLAG_STR(ARCH_SET_CPUID),
};

int log_ARCH_PRCTL_OPTION(uint64_t value)
{
	return option_log(value, arch_prctl_option_flags, sizeof(arch_prctl_option_flags) / sizeof(arch_prctl_option_flags[0]), "ARCH_???");
}
