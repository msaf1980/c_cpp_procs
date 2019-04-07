#include "netutils/netutils.h"
#include "strutils.h"
#include <string.h>
#include <stdlib.h>

int set_reuseaddr(int sock_fd)
{
	int enable = 1;
	return setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
}

int validate_ipv4(char *str)
{
	int dots = 0, digits = 0;
	char dbuf[4];

	if (str == NULL)
		return 0;

	while (1)
	{
		 /* after parsing string, it must contain only digits */
		if (*str == '\0' || *str == '.')
		{
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
			if (str2long(dbuf, &n, &tmp) != 0)
				return 0;
            if ((dots == 0 || dots == 3) && (n <= 0 || n >= 254))
                return 0;
            else if (n < 0 || n > 254)
                return 0;
			dots++;
		}
		else if (*str >= '0' && *str <= '9' && dots < 4)
			dbuf[digits++] = *str;
		else
			return 0;
		str++;
	}
}
