#ifndef _NETUTILS_H_
#define _NETUTILS_H_

#include <sys/types.h>
#ifdef __cplusplus
extern "C" {
#endif

#include <netinet/in.h>
#include <sys/socket.h>

/* socket helpers macros */
#define SA struct sockaddr
#define SA_IN struct sockaddr_in
#define SA_STOR struct sockaddr_storage

/* Set SO_REUSEADDR for listen socket */
int set_reuseaddr(int sock_fd);

/* return 0 if IPv4 string is valid, else return 1 */
int validate_ipv4(char *ip_str);

#ifdef __cplusplus
}
#endif

#endif /* _NETUTILS_H_ */
