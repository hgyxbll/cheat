#define CHEAT_OFFSET 43

#include <cheat.h>
#include <stdlib.h>

CHEAT_TEST(crash,
	exit(42);
)
