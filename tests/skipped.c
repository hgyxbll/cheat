#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <cheat.h>

CHEAT_SKIP(success, {})

CHEAT_SKIP(failure, {
	cheat_assert(0);
})
