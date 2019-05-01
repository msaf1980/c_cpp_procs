#ifndef _NETUTILS_H_
#define _NETUTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

/* socket helpers macros */
#define SA struct sockaddr
#define SA_IN struct sockaddr_in
#define SA_STOR struct sockaddr_storage

/* Set SO_REUSEADDR for listen socket */
int set_reuseaddr(int sock_fd);

/* errno set to EAGAIN in recv/send on timeout */
int set_recv_timeout(int sock_fd, struct timeval *tv);
int set_send_timeout(int sock_fd, struct timeval *tv);

/* Set SO_KEEPALIVE for socket */
int set_keepalive(int sock_fd);
/* Set TCP_KEEPIDLE */
int set_keepalive_idle(int sock_fd, int idle);
/* Set TCP_KEEPINTVL */
int set_keepalive_interval(int sock_fd, int interval);
/* Set TCP_KEEPCNT */
int set_keepalive_probes(int sock_fd, int probes);

/* try recv until end char on the end readed bytes or max_len reached */
ssize_t recv_try(int sock_fd, char *buf, size_t max_len, int flag, size_t *rsize, short *running, const char end);
/* try send until len reached */
ssize_t send_try(int sock_fd, char *buf, size_t len, int flag, size_t *wsize, short *running);

/* return 0 if IPv4 string is valid, else return 1 */
int validate_ipv4(char *ip_str);

#ifdef __cplusplus
}
#endif

#endif /* _NETUTILS_H_ */
