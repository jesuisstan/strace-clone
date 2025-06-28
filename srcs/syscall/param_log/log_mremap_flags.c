#define _GNU_SOURCE

#include "param_log.h"
#include <ft_printf.h>
#include <macros.h>
#include <sys/mman.h>

static const flag_str_t mmap_flags[] = {
	FLAG_STR(MREMAP_MAYMOVE),
	FLAG_STR(MREMAP_FIXED),
	FLAG_STR(MREMAP_DONTUNMAP),
};

/**
 * @brief log mremap flags
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_MREMAP_FLAGS(uint64_t value)
{
	return flags_log(value, mmap_flags, ELEM_COUNT(mmap_flags));
}