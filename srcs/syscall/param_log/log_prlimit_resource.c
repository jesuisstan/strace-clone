#include "param_log.h"
#include <sys/resource.h>

static const flag_str_t resources[] = {
	FLAG_STR(RLIMIT_AS),	  FLAG_STR(RLIMIT_CORE),	 FLAG_STR(RLIMIT_CPU),
	FLAG_STR(RLIMIT_DATA),	  FLAG_STR(RLIMIT_FSIZE),	 FLAG_STR(RLIMIT_LOCKS),
	FLAG_STR(RLIMIT_MEMLOCK), FLAG_STR(RLIMIT_MSGQUEUE), FLAG_STR(RLIMIT_NICE),
	FLAG_STR(RLIMIT_NOFILE),  FLAG_STR(RLIMIT_NPROC),	 FLAG_STR(RLIMIT_RSS),
	FLAG_STR(RLIMIT_RTPRIO),  FLAG_STR(RLIMIT_RTTIME),	 FLAG_STR(RLIMIT_SIGPENDING),
	FLAG_STR(RLIMIT_STACK),
};

int log_PRLIMIT_RESOURCE(uint64_t value)
{
	return option_log(value, resources, sizeof(resources) / sizeof(resources[0]), "RLIMIT_???");
}