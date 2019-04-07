#ifndef _DAEMONUTILS_H_
#define _DAEMONUTILS_H_

#ifdef __cplusplus
extern "C" {
#endif
    
/* !
Fork and init daemon child process
\param[in] foreground 0 - ze pprocess, 1 - run foreground
\param[in] nochdir 0 - chdir to / dir, 1 - don't chdir
\param[in] noclose 0 - close STDIN, STDOUT, STDERR, 1 - don't close
\return 0 - success in forked process, > 0 - success fork (returned child pid), -1 - failed
 */
int daemon_init(const int foreground, const int nochdir, const int noclose);

#ifdef __cplusplus
}
#endif

#endif /* _DAEMONUTILS_H_ */
