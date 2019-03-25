#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "procutil.h"

int main(void)
{
	int n_arg;
	
	char *cmd = "/bin/sh -c \"echo 123\"";
	char **arg = arg_parse(cmd, &n_arg, ' ');
	assert( n_arg == 3 );
	assert( strcmp(arg[0], "/bin/sh") == 0 );
	assert( strcmp(arg[1], "-c") == 0 );
	assert( strcmp(arg[2], "\"echo 123\"") == 0 );
	assert( arg[3] == NULL );
	arg_free(&arg);

	cmd = "name=ferret&color=purple";
	arg = arg_parse(cmd, &n_arg, '&');
	assert( n_arg == 2 );
	assert( strcmp(arg[0], "name=ferret") == 0 );
	assert( strcmp(arg[1], "color=purple") == 0 );
	assert( arg[2] == NULL );
	arg_free(&arg);
	
	return 0;
}