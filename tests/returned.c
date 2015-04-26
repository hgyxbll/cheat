#include <cheat.h>
#include <stdbool.h>

CHEAT_TEST(success,
	return;

	cheat_assert(false);
)

CHEAT_TEST(failure,
	cheat_assert(false);

	return;
)
