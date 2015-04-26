#include <cheat.h>
#include <stdbool.h>

CHEAT_TEST(success, {})

CHEAT_TEST(failure,
	cheat_assert(false);
)

CHEAT_TEST(another_failure,
	cheat_assert(CHEAT_GET(success) == CHEAT_GET(failure));

	CHEAT_CALL(success);
	CHEAT_CALL(failure);
)
