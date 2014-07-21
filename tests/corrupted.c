#include <cheat.h>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

CHEAT_TEST(overflow_crash,
	unsigned char* buffer;

	buffer = malloc(BUFSIZ);
	if (buffer != NULL)
		memset(&buffer[-BUFSIZ], UCHAR_MAX, BUFSIZ * 3);
	free(buffer);
)

CHEAT_TEST(assignment_crash,
	*(unsigned char* )NULL = UCHAR_MAX;
)
