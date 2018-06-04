/* For simple unit testing
 * Compatible with make output for better error handling in IDE/test editors (like VIM quickfix)
 * If use DWARN, exit with WARNCOUNT from main()
 
..
#define DASSERT_VERBOSE 1
#include <dassert.h>

static size_t WARNCOUNT = 0;


int main()
{
   ..
   DWARN(1 == 2, "Alwais fail");
   ..
   return WARNCOUNT;
}
 */

#ifndef DASSERT_VERBOSE
#define DASSERT_VERBOSE 0
#endif
 
#define DASSERT(x, msg) if (!(x)) { fprintf(stderr, "FAIL %s:%d: (%s) %s : %s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, #x, msg); abort(); }

#define DMSG(msg) fprintf(stderr, "%s:%d: (%s) : %s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, msg); }

#define DWARN(x, msg) if (!(x)) { fprintf(stderr, "FAIL %s:%d: (%s) %s : %s\n", \
                                        __FILE__, __LINE__, __PRETTY_FUNCTION__, #x, msg); WARNCOUNT++; } \
                      else if (DASSERT_VERBOSE) { fprintf(stderr, "PASS %s:%d: (%s) %s : %s\n", \
                                        __FILE__, __LINE__, __PRETTY_FUNCTION__, #x, msg); }

#define DWARN_U(x, msg, count) if (!(x)) { fprintf(stderr, "FAIL %s:%d: (%s) %s : %s (%lu)\n", \
                                        __FILE__, __LINE__, __PRETTY_FUNCTION__, #x, msg, count); WARNCOUNT++; } \
                      else if (DASSERT_VERBOSE) { fprintf(stderr, "PASS %s:%d: (%s) %s : %s\n", \
                                        __FILE__, __LINE__, __PRETTY_FUNCTION__, #x, msg); }

#define DWARN_S(x, msg, count) if (!(x)) { fprintf(stderr, "FAIL %s:%d: (%s) %s : %s (%ld)\n", \
                                        __FILE__, __LINE__, __PRETTY_FUNCTION__, #x, msg, count); WARNCOUNT++; } \
                      else if (DASSERT_VERBOSE) { fprintf(stderr, "PASS %s:%d: (%s) %s : %s\n", \
                                        __FILE__, __LINE__, __PRETTY_FUNCTION__, #x, msg); }
