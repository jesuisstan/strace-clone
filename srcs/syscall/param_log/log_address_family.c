#include "param_log.h"
#include <ft_printf.h>
#include <macros.h>
#include <sys/socket.h>
#include <unistd.h>

static const flag_str_t af_options[] = {
	FLAG_STR(AF_UNSPEC), FLAG_STR(AF_UNIX),		 FLAG_STR(AF_INET),		  FLAG_STR(AF_INET6),
	FLAG_STR(AF_IPX),	 FLAG_STR(AF_NETLINK),	 FLAG_STR(AF_X25),		  FLAG_STR(AF_AX25),
	FLAG_STR(AF_ATMPVC), FLAG_STR(AF_APPLETALK), FLAG_STR(AF_PACKET),	  FLAG_STR(AF_ALG),
	FLAG_STR(AF_ISDN),	 FLAG_STR(AF_PHONET),	 FLAG_STR(AF_IEEE802154), FLAG_STR(AF_CAIF),
	FLAG_STR(AF_CAN),	 FLAG_STR(AF_TIPC),		 FLAG_STR(AF_BLUETOOTH),  FLAG_STR(AF_IUCV),
	FLAG_STR(AF_RXRPC),	 FLAG_STR(AF_X25),		 FLAG_STR(AF_MAX),
};

/**
 * @brief Log address family
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_ADDRESS_FAMILY(uint64_t value)
{
	return option_log(value, af_options, ELEM_COUNT(af_options), "AF_???");
}