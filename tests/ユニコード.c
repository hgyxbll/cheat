#include <cheat.h>

CHEAT_TEST(success, {})

CHEAT_TEST(failure,
	cheat_assert(false);
)
