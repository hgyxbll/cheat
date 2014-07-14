#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <cheat.h>
#include <stdlib.h>

CHEAT_TEST(termination, {
	exit(42);
})
