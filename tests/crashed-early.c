#include <cheat.h>
#include <signal.h>

CHEAT_SET_UP(
	raise(SIGSEGV);
)

CHEAT_TEST(failure,
	cheat_assert(false);
)
