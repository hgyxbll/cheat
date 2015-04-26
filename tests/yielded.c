#include <cheat.h>
#include <stdbool.h>

CHEAT_DECLARE(
	static void f(void) {
		cheat_assert(false);
		cheat_yield();
		cheat_assert(false);
	}
)

CHEAT_TEST(success,
	cheat_yield();
)

CHEAT_TEST(failure,
	cheat_yield();
	cheat_assert(false);
	cheat_yield();
	cheat_assert(false);
)

CHEAT_TEST(another_failure,
	f();
	cheat_assert(false);
	cheat_yield();
	cheat_assert(false);
)
