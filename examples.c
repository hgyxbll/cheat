#include <cheat.h>
#include <cheats.h>
#include <stddef.h>
#include <string.h>

CHEAT_DECLARE(
	static char const yes[] = "yes";
	static char const times = '*';
	static int const thirteen = 13;
	static char const equals = '=';
	static double const a_lot = 1.21e+9;
)

CHEAT_TEST(confusing,
	cheat_assert(times == thirteen);
	cheat_assert(&equals == (char* )0xdead);
	cheat_assert(sizeof *yes != sizeof times);
)

CHEAT_TEST(obvious,
	cheat_assert_int(times, thirteen);
	cheat_assert_pointer(&equals, (char* )0xdead);
	cheat_assert_not_size(sizeof *yes, sizeof times);
)

CHEAT_DECLARE(
	static double f(double const x) {
		if (x < 0)
			return -x;

		return x;
	}
)

CHEAT_TEST(tedious,
	double const less = a_lot - thirteen;

	cheat_assert(strcmp(yes, "no") == 0);
	cheat_assert(f(a_lot - less) <= 1);
)

CHEAT_TEST(effortless,
	double const less = a_lot - thirteen;

	cheat_assert_string(yes, "no");
	cheat_assert_double(a_lot, less, 1);
)
