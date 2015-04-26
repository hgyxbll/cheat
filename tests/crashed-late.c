#include <cheat.h>
#include <signal.h>
#include <stdbool.h>

CHEAT_TEST(failure,
	cheat_assert(false);
)

CHEAT_TEAR_DOWN(
	raise(SIGSEGV);
)
