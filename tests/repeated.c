#include <cheat.h>

CHEAT_TEST(success,
	size_t index;

	for (index = 1;
			index < 13;
			++index)
		cheat_assert(index % 42);
)

CHEAT_TEST(failure,
	size_t index;

	for (index = 1;
			index < 42;
			++index)
		cheat_assert(index % 13);
)
