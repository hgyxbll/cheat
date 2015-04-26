#include <cheat.h>
#include <stdbool.h>

CHEAT_SKIP(success, {})

CHEAT_SKIP(failure,
	cheat_assert(false);
)
