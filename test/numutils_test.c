#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <c_procs/numutils.h>

#include <c_procs/dassert.h>

void int8_test() {
	DWARN(IS_INT8(127), "INT8 127");
	DWARN(IS_INT8(-128l), "INT8 -128");

	DWARN(!(IS_INT8(128)), "INT8 < 128");


	DWARN(IS_UINT8(0), "UINT8 0");
	DWARN(IS_UINT8(255), "UINT8 255");

	DWARN(!(IS_UINT8(256)), "UINT8 < 256");
}

void int16_test() {
	DWARN(IS_INT16(32767), "INT16 32767");
	DWARN(IS_INT16(-32768l), "INT16 -32768");

	DWARN(!(IS_INT16(32768)), "INT16 < 32768");


	DWARN(IS_UINT16(0), "UINT16 0");
	DWARN(IS_UINT16(65535), "UINT16 65535");

	DWARN(!(IS_UINT16(65536)), "UINT16 < 65536");
}

void int32_test() {
	DWARN(IS_INT32(2147483647), "INT32 2147483647");
	DWARN(IS_INT32(-2147483648l), "INT32 -2147483648");

	DWARN(!(IS_INT32(2147483648)), "INT32 < 2147483648");


	DWARN(IS_UINT32(0), "UINT32 0");
	DWARN(IS_UINT32(4294967295), "UINT32 4294967295");

	DWARN(!(IS_UINT32(4294967296)), "UINT32 < 4294967296");
}

int main(int argc, char *const *argv) {
	if (argc > 1 && strcmp(argv[1], "-q") == 0) {
		DWARN_VERBOSE = 0;
	} else {
		DWARN_VERBOSE = 1;
	}

	int8_test();
	int16_test();
	int32_test();

	return DSTATUS();
}
