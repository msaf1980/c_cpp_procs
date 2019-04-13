#include "netutils/netutils.h"
#include "strutils.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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
