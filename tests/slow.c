#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <cheat.h>
#include <unistd.h>

CHEAT_TEST(sleep, {
	sleep(1);
})

CHEAT_TEST(relax, {
	sleep(1);
})

CHEAT_TEST(nap, {
	sleep(1);
})
