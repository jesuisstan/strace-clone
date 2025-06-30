#include "param_log.h"
#include <sys/mman.h>

static const flag_str_t mmap_flags[] = {
	FLAG_STR(MAP_SHARED),	  FLAG_STR(MAP_PRIVATE),   FLAG_STR(MAP_FIXED),
	FLAG_STR(MAP_ANONYMOUS),  FLAG_STR(MAP_GROWSDOWN), FLAG_STR(MAP_DENYWRITE),
	FLAG_STR(MAP_EXECUTABLE), FLAG_STR(MAP_LOCKED),	   FLAG_STR(MAP_NORESERVE),
	FLAG_STR(MAP_POPULATE),	  FLAG_STR(MAP_NONBLOCK),  FLAG_STR(MAP_STACK),
	FLAG_STR(MAP_HUGETLB),	  FLAG_STR(MAP_SYNC),	   FLAG_STR(MAP_FIXED_NOREPLACE),
};

/**
 * @brief Log mmap flags
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_MMAP_FLAGS(uint64_t value)
{
	if (value == 0)
		return dprintf(STDERR_FILENO, "MAP_FILE");
	return flags_log(value, mmap_flags, sizeof(mmap_flags) / sizeof(mmap_flags[0]));
}