#define _GNU_SOURCE
#include "param_log.h"
#include <sys/personality.h>

static const flag_str_t personalities[] = {
	FLAG_STR(ADDR_COMPAT_LAYOUT), FLAG_STR(ADDR_NO_RANDOMIZE),
	FLAG_STR(ADDR_LIMIT_32BIT),	  FLAG_STR(ADDR_LIMIT_3GB),
	FLAG_STR(FDPIC_FUNCPTRS),	  FLAG_STR(MMAP_PAGE_ZERO),
	FLAG_STR(READ_IMPLIES_EXEC),  FLAG_STR(SHORT_INODE),
	FLAG_STR(STICKY_TIMEOUTS),	  FLAG_STR(UNAME26),
	FLAG_STR(WHOLE_SECONDS),	  FLAG_STR(PER_BSD),
	FLAG_STR(PER_HPUX),			  FLAG_STR(PER_IRIX32),
	FLAG_STR(PER_IRIX64),		  FLAG_STR(PER_IRIXN32),
	FLAG_STR(PER_ISCR4),		  FLAG_STR(PER_LINUX),
	FLAG_STR(PER_LINUX32),		  FLAG_STR(PER_LINUX32_3GB),
	FLAG_STR(PER_LINUX_32BIT),	  FLAG_STR(PER_LINUX_FDPIC),
	FLAG_STR(PER_OSF4),			  FLAG_STR(PER_OSR5),
	FLAG_STR(PER_RISCOS),		  FLAG_STR(PER_SCOSVR3),
	FLAG_STR(PER_SOLARIS),		  FLAG_STR(PER_SVR3),
	FLAG_STR(PER_SVR4),			  FLAG_STR(PER_UW7),
	FLAG_STR(PER_WYSEV386),		  FLAG_STR(PER_XENIX),
};

int log_PERSONALITY_FLAGS(uint64_t value)
{
	return flags_log(value, personalities, sizeof(personalities) / sizeof(personalities[0]));
}
