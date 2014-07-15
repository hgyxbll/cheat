#include <cheat.h>

CHEAT_TEST(success,
	cheat_assert(false);

	cheat_suite.outcome = CHEAT_SUCCESSFUL;
)

CHEAT_TEST(failure,
	cheat_suite.outcome = CHEAT_FAILED;
)
