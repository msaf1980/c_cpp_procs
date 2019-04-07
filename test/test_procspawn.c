#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <limits.h>

#include "procspawn.h"

#include <dassert.h>

static size_t WARNCOUNT = 0;

int main(void)
{
	char buf[PIPE_BUF + 1];
	ssize_t r = 0; 
	int status;

	char * const arg[] = {
		"/usr/bin/echo",
		"-v",
		"1",
		NULL
	};
	
	char * const env[] = {
		"TESTENV=test",
		NULL
	};

	struct proc_pipes p;
	PROC_PIPES_INIT(p);
	if ( (p.pid = proc_spawn(arg[0], NULL, NULL, p.pipes, 0)) == -1 ) {
	    perror(arg[0]); WARNCOUNT = EXIT_FAILURE; goto EXIT;
    }
    DWARN( proc_status( p.pid, &status ) >= 0 && WIFEXITED(status) && WEXITSTATUS(status) == 0, "run without arg" );
	r = read(p.pipes[1], buf, sizeof(buf));
	DWARN(r == 0, "run without arg: out");
	PROC_CLOSE(p, &status);

	if ( (p.pid = proc_spawn(arg[0], arg, NULL, p.pipes, 0)) == -1 ) {
	    perror(arg[0]); WARNCOUNT = EXIT_FAILURE; goto EXIT;
    }
	r = read(p.pipes[1], buf, sizeof(buf));
    DWARN(r > 0 && strcmp(buf, "-v 1\n") == 0, "run with arg: out");
	PROC_CLOSE(p, &status);

	if ( (p.pid = proc_spawn(arg[0], arg, env, p.pipes, 0)) == -1 ) {
	    perror(arg[0]); WARNCOUNT = EXIT_FAILURE; goto EXIT;
    }

	r = read(p.pipes[1], buf, sizeof(buf));
	DWARN(r > 0 && strcmp(buf, "-v 1\ntest\n") == 0, "run with arg and env");
	PROC_CLOSE(p, &status);

EXIT:
    if (WARNCOUNT)
        fprintf(stderr, "%lu tests failed\n", WARNCOUNT);
	return WARNCOUNT;
}
