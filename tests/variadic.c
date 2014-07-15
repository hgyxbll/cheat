#include <cheat.h>

#if __STDC_VERSION__ >= 199901L

CHEAT_DECLARE(
	enum question {
		ARE_YOU_FAT,
		ARE_YOU_SASSY
	};

	enum answer {
		YES,
		NO,
		MAYBE
	};

	enum question first, third;
)

CHEAT_SET_UP(
	first = ARE_YOU_FAT, third = ARE_YOU_FAT;
)

CHEAT_TEST(success,
	cheat_assert(first == third);
)

CHEAT_TEST(failure,
	enum answer second, fourth;

	cheat_assert((second = NO, fourth = MAYBE, second == fourth));
)

#else

CHEAT_DECLARE(CHEAT_COMMAS_4(
	enum question {
		ARE_YOU_FAT,
		ARE_YOU_SASSY
	};

	enum answer {
		YES,
		NO,
		MAYBE
	};

	enum question first, third;
))

CHEAT_SET_UP(
	CHEAT_COMMAS_1(first = ARE_YOU_FAT, third = ARE_YOU_FAT);
)

CHEAT_TEST(success,
	cheat_assert(first == third);
)

CHEAT_TEST(failure,
	enum answer second CHEAT_COMMA fourth;

	cheat_assert((second = NO, fourth = MAYBE, second == fourth));
)

#endif
