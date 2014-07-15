#include <cheat.h>
#include <signal.h>
#include <stddef.h>
#include <stdlib.h>

CHEAT_TEST(exit_crash,
	exit(EXIT_FAILURE);
)

CHEAT_TEST(abort_crash,
	abort();
)

CHEAT_TEST(termination_crash,
	raise(SIGTERM);
)

CHEAT_TEST(assignment_crash,
	*(unsigned char* )NULL = 0;
)

CHEAT_TEST(invocation_crash,
	((void (*)(void) )NULL)();
)

CHEAT_DECLARE(
	static volatile int number; /* This prevents optimizations. */
)

CHEAT_TEST(arithmetic_crash,
	number = 1 / 0;
)
