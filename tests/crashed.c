#include <cheat.h>
#include <signal.h>
#include <stddef.h>
#include <stdlib.h>

CHEAT_TEST(exit_crash,
	exit(EXIT_FAILURE);
)

CHEAT_TEST(termination_crash,
	raise(SIGTERM);
)

CHEAT_TEST(abortion_crash,
	abort();
)

CHEAT_DECLARE(
	static volatile int number; /* This prevents optimizations. */
)

CHEAT_SET_UP(
	number = 0;
)

CHEAT_TEST(arithmetic_crash,
	number = 1 / number;
)

CHEAT_TEST(invocation_crash,
	((void (*)(void) )NULL)();
)
