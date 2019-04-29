#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <netinet/tcp.h>

#include "netutils/netutils.h"
#include "strutils.h"

int set_reuseaddr(int sock_fd) {
    int reuse = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
        return -1;
#ifdef SO_REUSEPORT
    /* since Linux 3.9 */
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) < 0)
        return -1;
#endif
    return 0;
}

int set_recv_timeout(int sock_fd, struct timeval *tv) {
    return setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, tv, sizeof(struct timeval));
}

int set_send_timeout(int sock_fd, struct timeval *tv) {
    return setsockopt(sock_fd, SOL_SOCKET, SO_SNDTIMEO, tv, sizeof(struct timeval));
}

int set_keepalive(int sock_fd) {
    int enable = 1;
    return setsockopt(sock_fd, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(int));
}

int set_keepalive_idle(int sock_fd, int idle) {
    return setsockopt(sock_fd, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(int));
}

int set_keepalive_interval(int sock_fd, int interval) {
    return setsockopt(sock_fd, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(int));
}

int set_keepalive_probes(int sock_fd, int probes) {
    return setsockopt(sock_fd, IPPROTO_TCP, TCP_KEEPCNT, &probes, sizeof(int));
}

int validate_ipv4(char *str) {
    int dots = 0, digits = 0;
    char dbuf[4];

    if (str == NULL)
        return 0;

    while (1) {
        /* after parsing string, it must contain only digits */
        if (*str == '\0' || *str == '.') {
            if (digits == 0 || dots > 4)
                return 0;
            else if (*str == '\0' && (dots == 0 || dots == 3))
                return 1;
            else if (*str == '.' && dots == 3)
                return 0;
            dbuf[digits] = '\0';
            digits = 0;
            long n;
            char *tmp;
            n = str2l(dbuf, &tmp, 10);
            if (errno != 0)
                return 0;
            if ((dots == 0 || dots == 3) && (n <= 0 || n >= 254))
                return 0;
            else if (n < 0 || n > 254)
                return 0;
            dots++;
        } else if (*str >= '0' && *str <= '9' && dots < 4)
            dbuf[digits++] = *str;
        else
            return 0;
        str++;
    }
}
