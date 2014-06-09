#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <cheat.h>
#include <stddef.h>

CHEAT_TEST(crash, {
	((void (*)(void))NULL)();
})
