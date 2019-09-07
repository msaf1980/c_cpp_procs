#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <c_procs/procutils.h>

#include <c_procs/dassert.h>

void arg_parse_test() {
	int n_arg;

	const char *cmd = "/bin/sh  -c \"echo 123\"";
	char **     arg = arg_parse(cmd, &n_arg, ' ');
	DWARN(n_arg == 3, "arg count returned");
	if (n_arg == 3) {
		DWARN(strcmp(arg[0], "/bin/sh") == 0, arg[0]);
		DWARN(strcmp(arg[1], "-c") == 0, arg[1]);
		DWARN(strcmp(arg[2], "\"echo 123\"") == 0, arg[2]);
		DWARN(arg[3] == NULL, "NULL at the end");
	}
	arg_free(&arg);

	cmd = "/bin/sh";
	arg = arg_parse(cmd, &n_arg, ' ');
	DWARN(n_arg == 1, "arg count returned");
	if (n_arg == 1) {
		DWARN(strcmp(arg[0], "/bin/sh") == 0, arg[0]);
		DWARN(arg[1] == NULL, "NULL at the end");
	}
	arg_free(&arg);

	cmd = "name=ferret&color=purple";
	arg = arg_parse(cmd, &n_arg, '&');
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

	arg_parse_test();

	return DSTATUS();
}
