#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <c_procs/timeutils.h>

void timeset(struct timeval * tv, double seconds) {
	tv->tv_sec = (time_t) seconds;
	tv->tv_usec = (suseconds_t) ((seconds - tv->tv_sec) * 1000000);
}

