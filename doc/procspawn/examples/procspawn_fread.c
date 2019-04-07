#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "procspawn.h"

#define EC(cond, label, code, x) if (cond) { ec = code; x; goto label; }

#define BUFSIZE 512

int main(int argc, char *argv[])
{
	char *cmdline;
	pid_t pid;
	int pipes[3];
	int ec = 0;
	int rc = 0, status = 0;
	char buf[BUFSIZE+1];
	ssize_t r;
	FILE *fp;

	if (argc != 3)
		return 1;
	cmdline = malloc(strlen(argv[1]) + strlen(argv[2]) + 2);
	sprintf(cmdline, "%s %s", argv[1], argv[2]);
	EC( (pid = procspawn(cmdline, pipes)) == -1, EXIT, errno, perror("procspawn: ") ) 
	
	EC( (fp = fdopen(pipes[1], "r")) == NULL, CLEAN, errno, perror("stdout: ") )
	while ( (r  = fread(buf, sizeof(char), BUFSIZE, fp)) >= 0 ) {
		if (r == 0) {
			if (rc !=0 || (rc = procstatus(pid, &status)) != 0 ) {
				/* fprintf(stderr, "stdout ended\n"); */
				break;
			}
		}
		buf[r] = '\0';
		printf("%s\n", buf);
	}

	EC( (fp = fdopen(pipes[2], "r")) == NULL, CLEAN, errno, perror("stderr: ") )
	while ( (r  = fread(buf, sizeof(char), BUFSIZE, fp)) >= 0 ) {
		if (r == 0) {
			if (rc !=0 || (rc = procstatus(pid, &status)) != 0 ) {
				/* fprintf(stderr, "stderr ended\n"); */
				break;
			}
		}
		buf[r] = '\0';
		fprintf(stderr, "%s", buf);
	}
CLEAN:
	if (rc != 0)
		procclosepipes(pipes);
	else 
		rc = procclose(pid, pipes, &status);

	if (rc == -1)
		return(126);
	else if (WIFEXITED(status))
	        return(WEXITSTATUS(status));
	else
		return(127);
EXIT:
	return(ec);
}

