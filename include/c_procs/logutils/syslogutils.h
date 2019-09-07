/*
 * Macros for generic logging with printf-like format string
 * pass _logger as NULL
 */

#include <syslog.h>
#include <errno.h>
#include <string.h>

#define _LOG_EMERG(__logger, format, ...)  { syslog(LOG_EMERG, format, __VA_ARGS__); }
#define _LOG_FATAL(__logger, format, ...)  { syslog(LOG_ALERT, format, __VA_ARGS__); }
#define _LOG_ALERT(__logger, format, ...)  { syslog(LOG_ALERT, format, __VA_ARGS__); }
#define _LOG_CRIT(__logger, format, ...)  { syslog(LOG_CRIT, format, __VA_ARGS__); }
#define _LOG_ERROR(__logger, format, ...)  { syslog(LOG_ERR, format, __VA_ARGS__); }
#define _LOG_WARN(__logger, format, ...)  { syslog(LOG_WARNING, format, __VA_ARGS__); }
#define _LOG_NOTICE(__logger, format, ...)  { syslog(LOG_NOTICE, format, __VA_ARGS__); }
#define _LOG_INFO(__logger, format, ...)  { syslog(LOG_INFO, format, __VA_ARGS__); }
#define _LOG_DEBUG(__logger, format, ...)  { syslog(LOG_DEBUG, format, __VA_ARGS__); }

#define _LOG_ERROR_ERRNO(__logger, format, __errno, ...) { \
    char err_buf[1024]; \
    strerror_r((__errno), err_buf, sizeof(err_buf)); \
    syslog(LOG_ERR, format, __VA_ARGS__, err_buf); \
}

#define _LOG_FATAL_ERRNO(__logger, format, __errno, ...) { \
    char err_buf[1024]; \
    strerror_r((__errno), err_buf, sizeof(err_buf)); \
    syslog(LOG_ALERT, format, __VA_ARGS__,  err_buf); \
}
