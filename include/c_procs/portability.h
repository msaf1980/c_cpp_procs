/* Some fixes for portability */

#ifdef _GNU_SOURCE
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>

/* force switch to posix-compliant strerror_r instead of GNU for portability */
extern int __xpg_strerror_r(int errcode, char* buffer, size_t length);
#define strerror_r __xpg_strerror_r

/* thread-safe portable strerror */
inline char *strerror2(int errcode, char* buffer, size_t length) {
	if (strerror_r(errcode, buffer, length) != 0)
		snprintf(buffer, length, "failed to get error description: %d", errcode);
	return buffer;
}

#ifdef __cplusplus
}
#endif
#endif
