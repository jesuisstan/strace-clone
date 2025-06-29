#include "param_log.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

static const flag_str_t send_flags[] = {
	FLAG_STR(MSG_OOB),		FLAG_STR(MSG_DONTROUTE),	FLAG_STR(MSG_EOR),
	FLAG_STR(MSG_WAITALL),	FLAG_STR(MSG_CTRUNC),		FLAG_STR(MSG_TRUNC),
	FLAG_STR(MSG_DONTWAIT), FLAG_STR(MSG_NOSIGNAL),		FLAG_STR(MSG_MORE),
	FLAG_STR(MSG_CONFIRM),	FLAG_STR(MSG_ERRQUEUE),		FLAG_STR(MSG_RST),
	FLAG_STR(MSG_SYN),		FLAG_STR(MSG_CONFIRM),		FLAG_STR(MSG_WAITFORONE),
	FLAG_STR(MSG_FASTOPEN), FLAG_STR(MSG_CMSG_CLOEXEC),
};

/**
 * @brief Log send flags
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_SEND_FLAGS(uint64_t value)
{
	return flags_log(value, send_flags, sizeof(send_flags) / sizeof(send_flags[0]));
}