#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int daemon_init(const int foreground, const int nochdir, const int noclose)
{
	int fd0, fd1, fd2;
	int pid, i;

    if (foreground == 0) {
        if ( (pid = fork()) == -1) { /* fork failed */
            perror("fork");
            return(pid);
        } else if (pid > 0) { /* fork success */
            return(pid);
        }
    }

	if (! nochdir) {
		if (chdir("/") == -1) {
			perror("can't chdir to root dir");
			return -1;
		}
	}

	umask(027); /* Set file permissions 750 */

	/* Get a new process group */
	if (setsid() == -1) {
		perror("can't setsid");
		return -1;
	}

	/* Close  STDIN, STDOUT and STDERROR descriptors */
	if (! noclose) {
		for (i = 2; i >= 0; i--)
			close(i);

		/* Set file descriptors 0, 1 и 2 to /dev/null */
		 /* 'fd0' should be 0 */
		fd0 = open("/dev/null", O_RDWR);
		fd1 = dup(STDIN_FILENO);
		fd2 = dup(STDIN_FILENO);
	}

	return 0;
}
