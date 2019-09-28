#ifndef _DAEMONUTILS_H_
#define _DAEMONUTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define FD_NOCLOSE 0
#define FD_CLOSE_STD 1
#define FD_CLOSE_ALL 2

/*
 * Set the process's signal mask to ignore SIGPIPE signals. Returns 0
 * if success, -1 if error, in which case the errno code is left in
 * errno.
 */
int ignore_sigpipe();

/*
Fork and init daemon child process
	param[in] background 1 - fork and detach process, 0 - run foreground
	param[in] changedir 1 - chdir to / dir, 0 - don't chdir
	param[in] closefd 
		FD_NOCLOSE - don't close file descriptors
		FD_CLOSE_STD - close STDIN, STDOUT, STDERR
		FD_CLOSE_ALL - close all file descriptors
return
	0 - success in forked process or in foreground mode
	> 0 - success fork (returned child pid)
	-1 - failed
 */
int daemon_init(const int background, const int changedir, const int closefd);

#ifdef __cplusplus
}
#endif

#endif /* _DAEMONUTILS_H_ */
