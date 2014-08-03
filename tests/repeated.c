#define CHEAT_REPETITIONS 27

#include <cheat.h>
#include <stdlib.h>

CHEAT_REPEAT(success_maybe,
	cheat_assert(rand() % 42 != 0);
)

CHEAT_REPEAT(failure_maybe,
	cheat_assert(rand() % 13 != 0);
)
