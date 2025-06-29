#include "param_log.h"
#include <netinet/in.h>
#include <unistd.h>

static const flag_str_t ip_protos[] = {
	FLAG_STR(IPPROTO_IP),	 FLAG_STR(IPPROTO_ICMP),	FLAG_STR(IPPROTO_IGMP),
	FLAG_STR(IPPROTO_IPIP),	 FLAG_STR(IPPROTO_TCP),		FLAG_STR(IPPROTO_EGP),
	FLAG_STR(IPPROTO_PUP),	 FLAG_STR(IPPROTO_UDP),		FLAG_STR(IPPROTO_IDP),
	FLAG_STR(IPPROTO_TP),	 FLAG_STR(IPPROTO_DCCP),	FLAG_STR(IPPROTO_IPV6),
	FLAG_STR(IPPROTO_RSVP),	 FLAG_STR(IPPROTO_GRE),		FLAG_STR(IPPROTO_ESP),
	FLAG_STR(IPPROTO_AH),	 FLAG_STR(IPPROTO_MTP),		FLAG_STR(IPPROTO_BEETPH),
	FLAG_STR(IPPROTO_ENCAP), FLAG_STR(IPPROTO_PIM),		FLAG_STR(IPPROTO_COMP),
	FLAG_STR(IPPROTO_SCTP),	 FLAG_STR(IPPROTO_UDPLITE), FLAG_STR(IPPROTO_RAW),
};

/**
 * @brief Log ip protocol
 *
 * @param value the value to log
 * @return int the number of bytes written
 */
int log_IPPROTO(uint64_t value)
{
	return option_log(value, ip_protos, sizeof(ip_protos) / sizeof(ip_protos[0]), "IPPROTO_???");
}