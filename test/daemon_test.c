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
	int background = 1;

	pid = daemon_init(background, 1, 0);
	if (pid == 0) { /* child process */
		char cwd[PATH_MAX];
		if (getcwd(cwd, sizeof(cwd)) != NULL && strcmp(cwd, "/") == 0) {
			return 0;
		} else {
			return 1;
		}
	} else if (pid > 0) { /* parent process */
		int status;
		if (waitpid(pid, &status, 0) == -1) {
			fprintf(stderr, "waitpid failed: %s\n", strerror(errno));
			ec = 1;
		} else if (!WIFEXITED(status)) {
			fprintf(stderr, "child not exited\n");
			ec = 2;
		} else {
			ec = WEXITSTATUS(status);
			if (ec == 0) {
				printf("OK\n");
			} else {
				fprintf(stderr, "child exited with %d code\n", ec);
			}
		}

	} else { /* failed */
		ec = 1;
	}
EXIT:
	return ec;
}
