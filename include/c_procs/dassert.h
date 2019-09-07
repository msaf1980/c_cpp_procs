#ifndef _DASSERT_H_
#define _DASSERT_H_

/* For simple unit testing
 * Compatible with make output for better error handling in IDE/test editors
(like VIM quickfix)
 * If use DWARN, exit with DWARN_COUNT from main()

..

int main()
{
   DWARN_VERBOSE = 1;
   ..
   DWARN(1 == 2, "Alwais fail");
   ..
   return DWARN_COUNT;
}
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

static size_t DWARN_COUNT = 0;         /* test fail count */
static size_t DWARN_COUNT_SUCCESS = 0; /* test success count */

static size_t DWARN_VERBOSE = 0; /* verbose flag - print successed test */

#define DASSERT(x, msg)                                                        \
	if (!(x)) {                                                                \
		fprintf(stderr, "FAIL %s:%d: (%s) %s : %s\n", __FILE__, __LINE__,      \
		        __PRETTY_FUNCTION__, #x, msg);                                 \
		abort();                                                               \
	}

#define DMSG(msg)                                                              \
	{                                                                          \
		fprintf(stderr, "%s:%d: (%s) : %s\n", __FILE__, __LINE__,              \
		        __PRETTY_FUNCTION__, msg);                                     \
	}

#define DWARNMSG(msg)                                                          \
	{                                                                          \
		fprintf(stderr, "FAIL %s:%d: (%s) : %s\n", __FILE__, __LINE__,         \
		        __PRETTY_FUNCTION__, msg);                                     \
		DWARN_COUNT++;                                                         \
	}

#define DWARN(x, msg)                                                          \
	if (!(x)) {                                                                \
		fprintf(stderr, "FAIL %s:%d: (%s) %s : %s\n", __FILE__, __LINE__,      \
		        __PRETTY_FUNCTION__, #x, msg);                                 \
		DWARN_COUNT++;                                                         \
	} else {                                                                   \
		DWARN_COUNT_SUCCESS++;                                                 \
		if (DWARN_VERBOSE) {                                                   \
			fprintf(stderr, "PASS %s:%d: (%s) %s : %s\n", __FILE__, __LINE__,  \
			        __PRETTY_FUNCTION__, #x, msg);                             \
		} \
	}

/* verbose message with printf-like format string */
#define DWARN_F(x, format, ...)                                                \
	if (!(x)) {                                                                \
		fprintf(stderr, "FAIL %s:%d: (%s) %s : ", __FILE__, __LINE__,          \
				__PRETTY_FUNCTION__, #x);                                      \
		fprintf(stderr, format, __VA_ARGS__);                                  \
		fprintf(stderr, "\n");                                                 \
		DWARN_COUNT++;                                                         \
	} else {                                                                   \
		DWARN_COUNT_SUCCESS++;                                                 \
		if (DWARN_VERBOSE) {                                                   \
			fprintf(stderr, "PASS %s:%d: (%s) %s : ", __FILE__, __LINE__,      \
					__PRETTY_FUNCTION__, #x);                                  \
			fprintf(stderr, format, __VA_ARGS__);                              \
			fprintf(stderr, "\n");                                             \
		}                                                                      \
	}

int DSTATUS() {
	fprintf(stderr, "%lu test passed\n", DWARN_COUNT_SUCCESS);
	if (DWARN_COUNT)
		fprintf(stderr, "%lu tests failed\n", DWARN_COUNT);
	return DWARN_COUNT;
}

#ifdef __cplusplus
}
#endif

#endif /* _DASSERT_H_ */
