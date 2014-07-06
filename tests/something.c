#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <cheat.h>

CHEAT_TEST(success, {})

CHEAT_TEST(failure, {
	cheat_assert(0);
})

CHEAT_IGNORE(ignored, {
	cheat_assert(0);
})

CHEAT_SKIP(skipped, {
	cheat_assert(0);
})

CHEAT_TEST(another_success, {})

CHEAT_TEST(another_failure, {
	cheat_assert(0);
})

CHEAT_SKIP(another_skipped, {
	cheat_assert(0);
})
