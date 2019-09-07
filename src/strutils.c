#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <c_procs/strutils.h>

#ifndef __BSD_VISIBLE
size_t strlcpy(char *dst, const char *src, size_t dsize) {
	const char *osrc = src;
	size_t      nleft = dsize;

	/* Copy as many bytes as will fit. */
	if (nleft != 0) {
		while (--nleft != 0) {
			if ((*dst++ = *src++) == '\0')
				break;
		}
	}

	/* Not enough room in dst, add NUL and traverse rest of src. */
	if (nleft == 0) {
		if (dsize != 0)
			*dst = '\0'; /* NUL-terminate dst */
		while (*src++)
			;
	}

	return (src - osrc - 1); /* count does not include NUL */
}
#endif

char *sprintf_n(char *s, size_t maxsize, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	snprintf(s, maxsize, fmt, ap);
	return s;
}

int vsnprintf_l(char **p, size_t initsize, size_t maxsize, const char *fmt,
                ...) {
	int     n;
	size_t  size;
	char *  np;
	va_list ap;

	if (initsize == 0) {
		size = strlen(fmt);
		if (size < 512 && (maxsize == 0 || size * 2 <= maxsize))
			size *= 2;
	} else
		size = initsize;

	if (maxsize > 0 && size > maxsize)
		size = maxsize;

	if (*p == NULL && (*p = (char *) malloc(size)) == NULL)
		return -2;

	while (1) { /* Try to print in the allocated space */

		va_start(ap, fmt);
		n = vsnprintf(*p, size, fmt, ap);
		va_end(ap);

		/* Check error code */
		if (n < 0) {
			/*
			free(*p);
			*p = NULL;
			*/
			return n;
		}

		/* If that worked, return the string */
		if (n < size)
			return n;

		/* Else try again with more space */
		size = n + 1;                      /* Precisely what is needed */
		if (maxsize > 0 && size >= maxsize) /* maxsize */
			return n;
		if ((np = (char *) realloc(p, size)) == NULL) { /* realloc failed */
			/* free(p); */
			return -2;

		} else
			*p = np;
	}
}

long int str2l(const char *str, char **endptr, const int base) {
	errno = 0;
	long int n;
	n = strtol(str, endptr, base);
	if (endptr != NULL && **endptr != '\0') {
		if (n == 0 && strcmp(*endptr, str) == 0) {
			/* not a number */
			errno = EINVAL;
		} else {
			/* string not end */
			errno = ENOTEMPTY;
		}
	}
	return n;
}

long long int str2ll(const char *str, char **endptr, const int base) {
	errno = 0;
	long long int n;
	n = strtoll(str, endptr, base);
	if (endptr != NULL && **endptr != '\0') {
		if (n == 0 && strcmp(*endptr, str) == 0) {
			/* not a number */
			errno = EINVAL;
		} else {
			/* string not end */
			errno = ENOTEMPTY;
		}
	}
	return n;
}
