#include <cheat.h>
#include <stdio.h>

CHEAT_DECLARE(
	static int* number;
)

CHEAT_SET_UP(
	number = CHEAT_CAST(int*) malloc(sizeof *number);

	*number = 0;
)

CHEAT_TEST(success,
	cheat_assert(*number == 0);

	*number = 1;
)

CHEAT_TEST(another_success,
	cheat_assert(*number == 0);

	*number = 2;
)

CHEAT_TEAR_DOWN(
	free(number);
)
