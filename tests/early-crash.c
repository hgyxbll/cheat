#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <cheat.h>
#include <stdio.h>
#include <stdlib.h>

CHEAT_SET_UP({
	((void (*)(void))NULL)();
})

CHEAT_TEST(nothing, {
	puts("Not a test!");
})
