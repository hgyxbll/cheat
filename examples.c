#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <cheat.h>
#include <cheats.h>
#include <string.h>

CHEAT_TEST(assertions_are_unclear,
	char const yes[] = "yes";
	char const times = '*';
	int const thirteen = 13;

	cheat_assert(strcmp(yes, "no") == 0);
	cheat_assert(times == thirteen);
	cheat_assert(sizeof *yes != sizeof times);
)

CHEAT_TEST(additional_features_help,
	char const yes[] = "yes";
	char const times = '*';
	int const thirteen = 13;

	cheat_assert_string(yes, "no");
	cheat_assert_int(times, thirteen);
	cheat_assert_not_size(sizeof *yes, sizeof times);
)
