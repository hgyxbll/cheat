#include <cheat.h>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

CHEAT_TEST(assignment_crash,
	*(unsigned char* )NULL = UCHAR_MAX;
)

CHEAT_TEST(smash_crash,
	unsigned char buffer[1];

	memset(&buffer[-sizeof (int)], UCHAR_MAX, sizeof (int) * 3);
)

CHEAT_TEST(free_crash,
	unsigned char* buffer;

	buffer = malloc(BUFSIZ);
	free(buffer);
	free(buffer);
)
