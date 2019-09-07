//#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libgen.h>

#include <limits.h>

#include <c_procs/procspawn.h>

#include <c_procs/dassert.h>

int main(int argc, char *const *argv) {
	if (argc > 1 && strcmp(argv[1], "-q") == 0) {
		DWARN_VERBOSE = 0;
	} else {
		DWARN_VERBOSE = 1;
	}

	char buf[PIPE_BUF + 1];
	char test_prog[PATH_MAX];
	sprintf(buf, "%s", __FILE__);
	sprintf(test_prog, "%s/procspawn_test.pl", dirname(buf));
	ssize_t r = 0;
	int     status;

	char *const arg[] = {test_prog, "-v", "1", NULL};

	char *const env[] = {"PROCSPAWNENV=test", NULL};

	struct proc_pipes p;
	PROC_PIPES_INIT(p);
	if ((p.pid = proc_spawn(arg[0], NULL, NULL, p.pipes, 0)) == -1) {
		perror(arg[0]);
		DWARN_COUNT++;
		goto EXIT;
	}
	r = read(p.pipes[1], buf, sizeof(buf));
	if (r > 0)
		buf[r] = '\0';
	PROC_CLOSE(p, &status);
	DWARN(WIFEXITED(status) && WEXITSTATUS(status) == 0, "run without arg");
	DWARN(r == 0, "run without arg: out");

	if ((p.pid = proc_spawn(arg[0], arg, NULL, p.pipes, 0)) == -1) {
		perror(arg[0]);
		DWARN_COUNT++;
		goto EXIT;
	}
	r = read(p.pipes[1], buf, sizeof(buf));
	if (r > 0)
		buf[r] = '\0';
	PROC_CLOSE(p, &status);
	DWARN(WIFEXITED(status) && WEXITSTATUS(status) == 0, "run with arg");
	DWARN(r > 0 && strcmp(buf, "-v 1\n") == 0, "run with arg: out");

	if ((p.pid = proc_spawn(arg[0], arg, env, p.pipes, 0)) == -1) {
		perror(arg[0]);
		DWARN_COUNT++;
		goto EXIT;
	}
	r = read(p.pipes[1], buf, sizeof(buf));
	if (r > 0)
		buf[r] = '\0';
	PROC_CLOSE(p, &status);
	DWARN(WIFEXITED(status) && WEXITSTATUS(status) == 0,
	      "run with arg and env");
	DWARN(r > 0 && strcmp(buf, "-v 1\ntest\n") == 0, "run with arg and env");

EXIT:
	return DSTATUS();
}
