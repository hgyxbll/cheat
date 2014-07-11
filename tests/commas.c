#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <cheat.h>

CHEAT_DECLARE(CHEAT_COMMAS_2(
	enum answer {
		YES,
		NO,
		MAYBE
	};
))

CHEAT_TEST(success, {
	enum answer CHEAT_COMMAS_1(x, y);

	x = MAYBE CHEAT_COMMA y = MAYBE;
	cheat_assert(x == y);
})
