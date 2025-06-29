#define _GNU_SOURCE

#include "param_log.h"
#include <arpa/inet.h>
#include <ft_strace_utils.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/uio.h>

/**
 * @brief Log a sockaddr struct
 *
 * @param value the ptr to sockaddr
 * @param context the context of the syscall
 * @return int the number of bytes written
 */
int log_SOCKADDR_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct sockaddr, sockaddr);
	if (sockaddr.sa_family == AF_INET)
	{
		struct sockaddr_in *sockaddr_in = (struct sockaddr_in *)&sockaddr;
		return dprintf(STDERR_FILENO,
						  "{sa_family=AF_INET, sin_port=htons(%d), sin_addr=inet_addr(\"%s\")}",
						  ntohs(sockaddr_in->sin_port), inet_ntoa(sockaddr_in->sin_addr));
	}
	else if (sockaddr.sa_family == AF_INET6)
	{
		struct sockaddr_in6 *sockaddr_in6 = (struct sockaddr_in6 *)&sockaddr;
		char addr_str[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, &sockaddr_in6->sin6_addr, addr_str, INET6_ADDRSTRLEN);
		return dprintf(STDERR_FILENO,
						  "{sa_family=AF_INET6, sin6_port=htons(%d), sin6_addr=inet_addr(\"%s\")}",
						  ntohs(sockaddr_in6->sin6_port), addr_str);
	}
	else
		return dprintf(STDERR_FILENO, "{sa_family=%d}", sockaddr.sa_family);
}