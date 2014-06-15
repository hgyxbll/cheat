#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <cheat.h>

CHEAT_TEST(loop, {
	size_t index;

	for (index = 0;
			index < 1 << 14;
			++index)
		cheat_assert(index % (1 << 12));
})
