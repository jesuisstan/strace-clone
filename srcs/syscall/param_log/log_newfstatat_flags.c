#define _GNU_SOURCE

#include "param_log.h"
#include <fcntl.h>
#include <macros.h>

static const flag_str_t newfstatat_flags[] = {
	FLAG_STR(AT_EMPTY_PATH),
	FLAG_STR(AT_NO_AUTOMOUNT),
	FLAG_STR(AT_REMOVEDIR),
	FLAG_STR(AT_SYMLINK_FOLLOW),
};

int log_NEWFSTATAT_FLAGS(uint64_t value)
{
	return flags_log(value, newfstatat_flags, ELEM_COUNT(newfstatat_flags));
}