#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <limits.h>

#include "procspawn.h"

#include <dassert.h>

static size_t WARNCOUNT = 0;

int main(int argc, char * const * argv)
{
    if (argc != 2) {
        fprintf(stderr, "use: %s PATH/test_procspawn.pl\n", argv[0]);
        return EXIT_FAILURE;
    }
	char buf[PIPE_BUF + 1];
	ssize_t r = 0; 
	int status;

	char * const arg[] = {
		argv[1],
		"-v",
		"1",
		NULL
	};
	
	char * const env[] = {
		"PROCSPAWNENV=test",
		NULL
	};

	struct proc_pipes p;
	PROC_PIPES_INIT(p);
	if ( (p.pid = proc_spawn(arg[0], NULL, NULL, p.pipes, 0)) == -1 ) {
	    perror(arg[0]); WARNCOUNT = EXIT_FAILURE; goto EXIT;
    }
	r = read(p.pipes[1], buf, sizeof(buf));
    if (r > 0)
        buf[r] = '\0';
	PROC_CLOSE(p, &status);
    DWARN( WIFEXITED(status) && WEXITSTATUS(status) == 0, "run without arg" );
    DWARN( r == 0, "run without arg: out" );

	if ( (p.pid = proc_spawn(arg[0], arg, NULL, p.pipes, 0)) == -1 ) {
	    perror(arg[0]); WARNCOUNT = EXIT_FAILURE; goto EXIT;
    }
	r = read(p.pipes[1], buf, sizeof(buf));
    if (r > 0)
        buf[r] = '\0';
	PROC_CLOSE(p, &status);
    DWARN( WIFEXITED(status) && WEXITSTATUS(status) == 0, "run with arg" );
    DWARN( r > 0 && strcmp(buf, "-v 1\n") == 0, "run with arg: out" );

	if ( (p.pid = proc_spawn(arg[0], arg, env, p.pipes, 0)) == -1 ) {
	    perror(arg[0]); WARNCOUNT = EXIT_FAILURE; goto EXIT;
    }
	r = read(p.pipes[1], buf, sizeof(buf));
	if (r > 0)
        buf[r] = '\0';
	PROC_CLOSE(p, &status);
    DWARN( WIFEXITED(status) && WEXITSTATUS(status) == 0, "run with arg and env" );
    DWARN( r > 0 && strcmp(buf, "-v 1\ntest\n") == 0, "run with arg and env" );

EXIT:
    if (WARNCOUNT)
        fprintf(stderr, "%lu tests failed\n", WARNCOUNT);
	return WARNCOUNT;
}
