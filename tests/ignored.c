#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <cheat.h>

CHEAT_IGNORE(success, {})

CHEAT_IGNORE(failure, {
	cheat_assert(0);
})
