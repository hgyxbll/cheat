#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <cheat.h>

CHEAT_TEST(failure, {
	cheat_assert(0);
	cheat_suite->tests_successful = (size_t )-1;
	cheat_suite->tests_failed = (size_t )-1;
	cheat_suite->tests_run = (size_t )-1;
	cheat_suite->outcome = CHEAT_SUCCESS;
})
