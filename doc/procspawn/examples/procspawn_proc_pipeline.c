#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include <limits.h>

#include "procspawn.h"
#include "procutils.h"

#define EC(cond, label, code, x)                                               \
    if (cond) {                                                                \
        ec = code;                                                             \
        x;                                                                     \
        goto label;                                                            \
    }
#define EC_ERRNO(cond, label, x)                                               \
    if (cond) {                                                                \
        ec = errno;                                                            \
        x;                                                                     \
        goto label;                                                            \
    }

int set_nonblock(int fd) {
    int flags;
/* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
#if defined(O_NONBLOCK)
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    flags = 1;
    return ioctl(fd, FIOBIO, &flags);
#endif
}

int main(int argc, char **argv) {
    char buf[PIPE_BUF];
    ssize_t r = 0;
    int ec = 0, i, status;

    const int n = 3;
    char *cmd[] = {"who", "sort", "uniq -c", "sort -nk1"};
    struct proc_pipes p[4];
    for (i = 0; i <= n; i++)
        PROC_PIPES_INIT(p[i]);

    for (i = 0; i <= n; i++) {
        int n_arg;
        char **arg = arg_parse(cmd[i], &n_arg, ' ');
        // EC_ERRNO( (pe->p->pid = proc_spawn(pe->cmd, NULL, pe->p->pipes)) ==
        // -1, EXIT, perror(pe->cmd));
        EC_ERRNO((p[i].pid = proc_spawn(*arg, arg, p[i].pipes, 0)) == -1, EXIT,
                 perror(cmd[i]));
        arg_free(&arg);
        set_nonblock(p[i].pipes[1]);
    }

    while (1)
        for (i = 0; i <= n; i++) {
            if (i > 0 && r > 0 && p[i].pipes[0] != -1) {
                if (write(p[i].pipes[0], buf, r) != r) {
                    fprintf(stderr, "error: write to stdout: %s\n", cmd[i]);
                    goto EXIT;
                }
            }
            if (p[i].pipes[1] == -1)
                r = 0;
            else {
                // close(p[i].pipipes[1]);
                // p[i].pipes[1] = -1;
                short closed = 0;
                r = read(p[i].pipes[1], buf, sizeof(buf));
                if (r == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                    /*
                    if (proc_status(p[i].pid, &status)) != 0)
                            closed = 1;
                    */
                    break;
                } else if (r == 0 || r == -1) { /* close stdout of process */
                    closed = 1;
                }

                if (closed) {
                    close(p[i].pipes[1]);
                    p[i].pipes[1] = -1;
                    if (i != n) { /* close stdin of next process, no more data
                                     to write */
                        close(p[i + 1].pipes[0]);
                        p[i + 1].pipes[0] = -1;
                    } else if (i == n)
                        goto EXIT;
                }
            }
            if (i == n && r > 0)
                printf("%s", buf);
        }

EXIT:
    for (i = 0; i <= n; i++)
        PROC_CLOSE(p[i], NULL);
    return ec;
}
