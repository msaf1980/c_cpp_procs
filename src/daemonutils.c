#include <fcntl.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <daemonutils.h>

int daemon_init(const int background, const int changedir, const int closefd) {
    int fd0, fd1, fd2;
    int pid, i;

    if (background) {
        if ((pid = fork()) == -1) { /* fork failed */
            perror("fork");
            return (pid);
        } else if (pid > 0) { /* fork success */
            return (pid);
        }
        /* Get a new process group */
        if (setsid() == -1) {
            perror("can't setsid");
            return -1;
        }
    }

    if (changedir) {
        if (chdir("/") == -1) {
            perror("can't chdir to root dir");
            return -1;
        }
    }

    umask(027); /* Set file permissions 750 */

    /* Close  STDIN, STDOUT and STDERROR descriptors */
    if (closefd != FD_NOCLOSE) {
        size_t maxclosefd;
        if (closefd == FD_CLOSE_STD)
            maxclosefd = 2;
        else {
            /* Get maximum number of file descriptors. */
            struct rlimit rl;
            if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
                perror("can't get file limit");
                return -1;
            }
            if (rl.rlim_max == RLIM_INFINITY)
                maxclosefd = 1024;
            else
                maxclosefd = rl.rlim_max;
        }

        for (i = maxclosefd; i >= 0; i--)
            close(i);

        /* Set file descriptors 0, 1 и 2 to /dev/null */
        /* 'fd0' should be 0 */
        fd0 = open("/dev/null", O_RDWR);
        fd1 = dup(STDIN_FILENO);
        fd2 = dup(STDIN_FILENO);
    }

    return 0;
}
