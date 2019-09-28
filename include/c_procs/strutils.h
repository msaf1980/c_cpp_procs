#ifndef _STRUTILS_H_
#define _STRUTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#ifndef __BSD_VISIBLE
/*
 * Copy string src to buffer dst of size dsize.  At most dsize-1
 * chars will be copied.  Always NUL terminates (unless dsize == 0).
 * Returns strlen(src); if retval >= dsize, truncation occurred.
 */
size_t strlcpy(char *dst, const char *src, size_t dsize);
#endif

/*
 * sprintf_n like snprintf, but without truncate checking
 * @param s - buffer
 * @param maxsize - limit string len
 * @param fmt - format string
 * @param .. VARAGS
 * @return buffer
 */
char *sprintf_n(char *s, size_t maxsize, const char *fmt, ...);

/*
 * vsnprintf with dynamically allocated buffer (need to bee free, if function
 * return value >=0)
 * @param p - pointer to allocated buffer
 *      free memory before pass pointer or will be !!! MEMORY LEAK !!! after
 * allocation)
 * @param initsize - initial size for allocated buffer, set 0 to autosize
 * @param maxsize - limit allocated memory, set 0 to unlimit
 * @param fmt - format string
 * @param .. VARAGS
 * @return like vsnprintf
 *      >= maxsize (maxsize > 0) Maxsize too small
 *      < 0 output error
 *      -2 allocation error
 *      > 0,  < maxsize (maxsize > 0) Success
 *
 * use like
 *   char *p = NULL;
 *   if  (vsnprintf_l(&p, 0, "c = %d, n = %d\n", c, n) > 0)
 *   {
 *     ..
 *     free(p);
 *   }
 *
 *
 */
int vsnprintf_l(char **p, size_t initsize, size_t maxsize, const char *fmt,
                ...);

/*
 * Additional checks for strtol and etc. errno set to EINVAL, if **endptr not
 * '\0' on error, errno was set EINVAL, not a number ERANGE, value is out of
 * range EINVAL, value is invalid
 */
long int      str2l(const char *str, char **endptr, const int base);
long long int str2ll(const char *str, char **endptr, const int base);

/* split sring, after use free with arg_free */
char **arg_split(const char *str, int *n, char delim);
/* free allocated by arg_split */
void arg_free(char ***p);

#ifdef __cplusplus
}
#endif

#endif /* _STRUTILS_H_ */
