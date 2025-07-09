#include "param_log.h"
#include <sys/mman.h>

static const flag_str_t mmap_prot_flags[] = {
	FLAG_STR(PROT_READ),
	FLAG_STR(PROT_WRITE),
	FLAG_STR(PROT_EXEC),
};

/**
 * @brief Log mmap protection flags
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_MEM_PROT(uint64_t value)
{
	if (value == 0)
		return dprintf(STDERR_FILENO, "PROT_NONE");
	return flags_log(value, mmap_prot_flags, sizeof(mmap_prot_flags) / sizeof(mmap_prot_flags[0]));
}