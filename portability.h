/* Some fixes for portability */

#ifdef _GNU_SOURCE
#ifdef __cplusplus
extern "C"
{
#endif

/* force switch to posix-compliant strerror_r instead of GNU for portability */
extern int __xpg_strerror_r(int errcode, char* buffer, size_t length);
#define strerror_r __xpg_strerror_r

#ifdef __cplusplus
}
#endif
#endif
