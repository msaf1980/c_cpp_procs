#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <c_procs/strutils.h>

#include <c_procs/dassert.h>

void strlcpy_test() {
	char   src[10];
	char   dst[10];
	char   buf[1024];
	size_t n, s;

	strcpy(src, "test");
	strcpy(dst, "123456789");
	s = strlen(src);

	n = strlcpy(dst, src, s + 2);
	DWARN(strcmp(dst, src) == 0, dst);
	DWARN_F(n == s, "source length %lu", n);

	n = strlcpy(dst, src, s);
	DWARN(strcmp(dst, "tes") == 0, dst);
	DWARN_F(n == s, "source length %lu", n);

	n = strlcpy(dst, src, 0);
	DWARN_F(n == s, "source length %lu", n);
}

void vsnprintf_l_test() {
	char *buf = NULL;
	int   n;

	n = vsnprintf_l(&buf, 0, 3, "%s %s", "1", "2");
	DWARN_F(buf != NULL && strcmp(buf, "1 ") == 0, "'%s'", buf);
	DWARN_F(n == 3, "required maxsize is %d", n + 1);

	n = vsnprintf_l(&buf, 0, 1024, "%s %s", "1", "2");
	DWARN_F(buf != NULL && strcmp(buf, "1 2") == 0, "'%s'", buf);
	DWARN_F(n == 3, "size is %d", n);

	free(buf);
}

void str2l_test() {
	char *      endptr;
	long int    n;
	const char *buf;

	/* overflow check */
	buf = "922337203685477580700000";
	n = str2l(buf, &endptr, 10);
	DWARN_F(errno == ERANGE, "'%s' -> %ld", buf, n);

	/* not a number check */
	buf = "a1";
	n = str2l(buf, &endptr, 10);
	DWARN_F(errno == EINVAL, "'%s' -> %ld", buf, n);

	buf = "a 1";
	n = str2l(buf, &endptr, 10);
	DWARN_F(errno == EINVAL, "'%s' -> %ld", buf, n);

	buf = " 1 a";
	n = str2l(buf, &endptr, 10);
	DWARN_F(errno == ENOTEMPTY && n == 1l && strcmp(endptr, " a") == 0,
	        "'%s' -> %ld ('%s')", buf, n, endptr);

	buf = " 1 ";
	n = str2l(buf, &endptr, 10);
	DWARN_F(errno == ENOTEMPTY && n == 1l && strcmp(endptr, " ") == 0,
	        "'%s' -> %ld ('%s')", buf, n, endptr);

	/* valid long */
	buf = "10";
	n = str2l(buf, &endptr, 10);
	DWARN_F(errno == 0 && n == 10l, "'%s' -> %ld", buf, n);

	buf = "-1";
	n = str2l(buf, &endptr, 10);
	DWARN_F(errno == 0 && n == -1l, "'%s' -> %ld", buf, n);
}

void str2ll_test() {
	char *        endptr;
	long long int n;
	const char *  buf;

	/* overflow check */
	buf = "922337203685477580700000";
	n = str2l(buf, &endptr, 10);
	DWARN_F(errno == ERANGE, "'%s' -> %ld", buf, n);

	/* not a number check */
	buf = "a1";
	n = str2l(buf, &endptr, 10);
	DWARN_F(errno == EINVAL, "'%s' -> %ld", buf, n);

	buf = "a 1";
	n = str2l(buf, &endptr, 10);
	DWARN_F(errno == EINVAL, "'%s' -> %ld", buf, n);

	buf = " 1 a";
	n = str2l(buf, &endptr, 10);
	DWARN_F(errno == ENOTEMPTY && n == 1l && strcmp(endptr, " a") == 0,
	        "'%s' -> %ld ('%s')", buf, n, endptr);

	buf = " 1 ";
	n = str2l(buf, &endptr, 10);
	DWARN_F(errno == ENOTEMPTY && n == 1l && strcmp(endptr, " ") == 0,
	        "'%s' -> %ld ('%s')", buf, n, endptr);

	/* valid long */
	buf = "10";
	n = str2l(buf, &endptr, 10);
	DWARN_F(errno == 0 && n == 10l, "'%s' -> %ld", buf, n);

	buf = "-1";
	n = str2l(buf, &endptr, 10);
	DWARN_F(errno == 0 && n == -1l, "'%s' -> %ld", buf, n);
}

void arg_split_test() {
        int n_arg;

        const char *cmd = "/bin/sh  -c \"echo 123\"";
        char **     arg = arg_split(cmd, &n_arg, ' ');
        DWARN(n_arg == 3, "arg count returned");
        if (n_arg == 3) {
                DWARN(strcmp(arg[0], "/bin/sh") == 0, arg[0]);
                DWARN(strcmp(arg[1], "-c") == 0, arg[1]);
                DWARN(strcmp(arg[2], "\"echo 123\"") == 0, arg[2]);
                DWARN(arg[3] == NULL, "NULL at the end");
        }
        arg_free(&arg);

        cmd = "/bin/sh";
        arg = arg_split(cmd, &n_arg, ' ');
        DWARN(n_arg == 1, "arg count returned");
        if (n_arg == 1) {
                DWARN(strcmp(arg[0], "/bin/sh") == 0, arg[0]);
                DWARN(arg[1] == NULL, "NULL at the end");
        }
        arg_free(&arg);

        cmd = "name=ferret&color=purple";
        arg = arg_split(cmd, &n_arg, '&');
        DWARN(n_arg == 2, "arg count returned");
        if (n_arg == 2) {
                DWARN(strcmp(arg[0], "name=ferret") == 0, arg[0]);
                DWARN(strcmp(arg[1], "color=purple") == 0, arg[1]);
                DWARN(arg[2] == NULL, "NULL at the end");
        }
        arg_free(&arg);
}

int main(int argc, char *const *argv) {
	if (argc > 1 && strcmp(argv[1], "-q") == 0) {
		DWARN_VERBOSE = 0;
	} else {
		DWARN_VERBOSE = 1;
	}

	strlcpy_test();
	vsnprintf_l_test();
	str2l_test();
	str2ll_test();
	arg_split_test();

	return DSTATUS();
}
