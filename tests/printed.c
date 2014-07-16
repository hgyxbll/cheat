#include <cheat.h>
#include <stdio.h>

CHEAT_SET_UP(
	fputs("Set up began.\n", stderr);

	fputs("Set up!\n", stdout);

	fputs("Set up ended.\n", stderr);
)

CHEAT_TEST(success,
	fputs("Test began.\n", stderr);

	fputs("Test!\n", stdout);

	fputs("Test ended.\n", stderr);
)

CHEAT_TEST(failure,
	fputs("Test began.\n", stderr);

	fputs("Test!\n", stdout);
	cheat_assert(false);

	fputs("Test ended.\n", stderr);
)

CHEAT_TEAR_DOWN(
	fputs("Tear down began.\n", stderr);

	fputs("Tear down!\n", stdout);

	fputs("Tear down ended.\n", stderr);
)
