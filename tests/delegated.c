#include <cheat.h>

CHEAT_DECLARE(
	static void f(int const x) {
		cheat_assert(x * x + x - 2 == 0);
	}
)

CHEAT_TEST(success,
	f(1);
)
CHEAT_TEST(failure,
	f(2);
)
