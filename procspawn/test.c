#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <limits.h>

#include "procspawn.h"

#define EC_ERRNO(cond, label, x) if (cond) { ec = errno; x; goto label; } 

int main(void)
{
	char buf[PIPE_BUF + 1];
	ssize_t r = 0; 
	int status, ec  = 0;

	char * const arg[] = {
		"./test.pl",
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
	EC_ERRNO( (p.pid = proc_spawn(arg[0], NULL, NULL, p.pipes, 0)) == -1, EXIT, perror(arg[0]) ); 
	r = read(p.pipes[1], buf, sizeof(buf));
	assert(r == 0);
	PROC_CLOSE(p, &status);

	EC_ERRNO( (p.pid = proc_spawn(arg[0], arg, NULL, p.pipes, 0)) == -1, EXIT, perror(arg[0]) ); 
	r = read(p.pipes[1], buf, sizeof(buf));
	assert(r > 0 && strcmp(buf, "-v 1\n") == 0);
	PROC_CLOSE(p, &status);

	EC_ERRNO( (p.pid = proc_spawn(arg[0], arg, env, p.pipes, 0)) == -1, EXIT, perror(arg[0]) ); 
	r = read(p.pipes[1], buf, sizeof(buf));
	assert(r > 0 && strcmp(buf, "-v 1\ntest\n") == 0);
EXIT:
	PROC_CLOSE(p, &status);
	return ec;
}
