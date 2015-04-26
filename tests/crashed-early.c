#include <cheat.h>
#include <signal.h>
#include <stdbool.h>

CHEAT_SET_UP(
	raise(SIGSEGV);
)

CHEAT_TEST(failure,
	cheat_assert(false);
)
