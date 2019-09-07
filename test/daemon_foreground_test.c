#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <sys/wait.h>

#include <c_procs/daemonutils.h>

int main(int argc, char *const argv[]) {
	int ec = 0, pid;
	int background = 0;

	pid = daemon_init(background, 1, 0);
	if (pid == 0) { /* child process */
		char cwd[PATH_MAX];
		if (getcwd(cwd, sizeof(cwd)) != NULL && strcmp(cwd, "/") == 0) {
			printf("OK\n");
			return 0;
		} else {
			return 1;
		}
	} else if (pid > 0) { /* parent process */
		fprintf(stderr, "fork detected\n");
		return 2;
	} else { /* failed */
		ec = 1;
	}
EXIT:
	return ec;
}
