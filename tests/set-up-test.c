#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <cheat.h>
#include <stdio.h>

CHEAT_SET_UP({
	puts("Set up!");
})

CHEAT_TEAR_DOWN({
	puts("Tear down!");
})

CHEAT_TEST(test, {
	puts("Test!");
})
