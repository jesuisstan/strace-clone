#include "param_log.h"
#include <ft_printf.h>
#include <macros.h>
#include <sys/socket.h>
#include <unistd.h>

static const flag_str_t socket_types[] = {
	FLAG_STR(SOCK_STREAM), FLAG_STR(SOCK_DGRAM),	 FLAG_STR(SOCK_RAW),
	FLAG_STR(SOCK_RDM),	   FLAG_STR(SOCK_SEQPACKET), FLAG_STR(SOCK_DCCP),
	FLAG_STR(SOCK_PACKET), FLAG_STR(SOCK_CLOEXEC),	 FLAG_STR(SOCK_NONBLOCK),
};

/**
 * @brief Log socket types
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_SOCKET_TYPE(uint64_t value)
{
	return flags_log(value, socket_types, ELEM_COUNT(socket_types));
}