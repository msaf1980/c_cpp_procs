#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
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

#define SIZE_INC 10

char **arg_split(const char *str, int *n, char delim) {
    const char *p, *s;
    char **new_arg;
    short space;

    int asize = SIZE_INC; /* allocated_size - 1 */
    char **arg = malloc((asize + 1) * sizeof(char *));
    if (arg == NULL)
        return NULL;

    *n = 0;
    space = 1;
    s = NULL;
    p = str;
    while (1) {
        if (*p == '\0' && s == NULL)
            break;
        else if (*p == '\0' || (*p == delim && space && s != NULL)) {
            if (*n == asize) {
                asize += SIZE_INC + 1;
                new_arg = realloc(arg, (asize + 1) * sizeof(char *));
                if (new_arg == NULL) {
                    arg_free(&arg);
                    return NULL;
                } else
                    arg = new_arg;
            }
            arg[*n] = strndup(s, p - s);
            (*n)++;
            if (*p == '\0')
                break;
            s = NULL;
        } else if (*p != delim) {
            if (*p == '"') {
                if (space) {
                    space = 0;
                } else
                    space = 1;
            }
            if (s == NULL)
                s = p;
        }
        p++;
    }
    arg[*n] = NULL;

    if (*n < asize) {
        new_arg = realloc(arg, (*n + 1) * sizeof(char *));
        if (new_arg != NULL)
            return new_arg;
    }
    return arg;
}

void arg_free(char ***str) {
    char **p;
    if (*str == NULL)
        return;
    p = *str;
    while (*p != NULL) {
        free(*p);
        p++;
    }
    free(*str);
}

