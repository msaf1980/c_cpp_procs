#include <errno.h>
#include <stdarg.h>
#include <strutils.h>

int vsnprintf_l(char **p, size_t initsize, size_t maxsize, const char *fmt,
                ...) {
  int n;
  size_t size;
  char *np;
  va_list ap;

  if (initsize == 0) {
    size = strlen(fmt);
    if (size < 512 && (maxsize == 0 || size * 2 <= maxsize))
      size *= 2;
  } else
    size = initsize;

  if (maxsize > 0 && size > maxsize)
    return -2;

  if (*p != NULL && initsize == 0)
    return -2;

  if (*p == NULL && (*p = (char *)malloc(size)) == NULL)
    return -2;

  while (1) { /* Try to print in the allocated space */

    va_start(ap, fmt);
    n = vsnprintf(*p, size, fmt, ap);
    va_end(ap);

    /* Check error code */
    if (n < 0) {
      free(*p);
      *p = NULL;
      return n;
    }

    /* If that worked, return the string */
    if (n < size)
      return n;

    /* Else try again with more space */
    size = n + 1;                      /* Precisely what is needed */
    if (maxsize > 0 && size > maxsize) /* maxsize */
      return n;
    if ((np = (char *)realloc(p, size)) == NULL) { /* realloc failed */
      free(p);
      return -2;
    } else
      *p = np;
  }
}

int str2long(const char *str, long int *n, char **temp) {
  errno = 0;
  *n = strtol(str, temp, 10);
  if (*n == 0 && temp != NULL && *temp != '\0') {
    /* value must be a number, wrong sequence in temp */
    errno = EINVAL;
  }
  return errno;
}
