#include <cheat.h>

CHEAT_IGNORE(success, {})

CHEAT_IGNORE(failure,
	cheat_assert(false);
)
