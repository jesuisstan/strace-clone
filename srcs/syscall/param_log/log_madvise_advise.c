#include "param_log.h"
#include <sys/mman.h>

static const flag_str_t madvise_options[] = {
	FLAG_STR(MADV_NORMAL),	   FLAG_STR(MADV_RANDOM),	   FLAG_STR(MADV_SEQUENTIAL),
	FLAG_STR(MADV_WILLNEED),   FLAG_STR(MADV_DONTNEED),	   FLAG_STR(MADV_REMOVE),
	FLAG_STR(MADV_DONTFORK),   FLAG_STR(MADV_DOFORK),	   FLAG_STR(MADV_HWPOISON),
	FLAG_STR(MADV_MERGEABLE),  FLAG_STR(MADV_UNMERGEABLE), FLAG_STR(MADV_HUGEPAGE),
	FLAG_STR(MADV_NOHUGEPAGE), FLAG_STR(MADV_DONTDUMP),	   FLAG_STR(MADV_DODUMP)};

/**
 * @brief Log madvise advice
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_MADVISE_ADVISE(uint64_t value)
{
	return option_log(value, madvise_options, sizeof(madvise_options) / sizeof(madvise_options[0]), "MADV_???");
}