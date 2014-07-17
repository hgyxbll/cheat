#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <cheat.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

CHEAT_DECLARE(
	extern size_t size;
)

CHEAT_TEST(mathematics_still_work,
	cheat_assert(2 + 2 == 4);
	cheat_assert(2 + 2 != 5);
)

CHEAT_DECLARE(
	static double f(double const x, double const y) {
		return 2 / (x * x) - y * y / 2;
	}

	static bool p(double const x, double const y, double const e) {
		double d;

		if (x < y)
			d = y - x;
		else /* Turbo C would parse this wrong without this comment. */
			d = x - y;
		return d <= e;
	}
)

CHEAT_TEST(physics_still_work,
	double x0;
	double y0;
	size_t n;
	double xn;
	double dx;
	double y;
	double s;
	size_t i;
	double x;

	x0 = 1;
	y0 = 2;
	n = 128;
	xn = 3;
	dx = xn - x0;
	y = y0;
	s = dx / (double )n;
	for (i = 0;
			i <= n;
			++i) {
		x = x0 + s * (double )i;
		y += s * f(x, y);
	}

	cheat_assert(p(y, 1, 0.1));
)

CHEAT_DECLARE(
	size_t size;
	static char* heap;
)

CHEAT_SET_UP(
	char const* stack;

	fputs("Setting up", stderr);

	stack = "string";
	size = strlen(stack) + 1;
	heap = CHEAT_CAST(char*) malloc(size);
	memcpy(heap, stack, size);
)

CHEAT_TEAR_DOWN(
	fputs(" and tearing down.\n", stderr);

	free(heap);
)

CHEAT_TEST(philosophy_never_worked,
	char const* stack;

	stack = "string";
	cheat_assert(heap == stack);
	cheat_assert(strcmp(heap, stack) == 0);
)

CHEAT_TEST(test,
	fputs(", running a test", stderr);
)

#ifndef OXYGEN_MOLECULE
#define OXYGEN_MOLECULE (0 == 0)
#endif

CHEAT_TEST(chemistry_is_strange,
	cheat_assert(OXYGEN_MOLECULE);
)

CHEAT_DECLARE(
	enum things {
		THIS_TEST CHEAT_COMMA
		IMPORTANT_TEST
	};
)

CHEAT_IGNORE(important,
	cheat_assert(THIS_TEST == IMPORTANT_TEST);
)

CHEAT_IGNORE(unimportant,
	cheat_assert(THIS_TEST != IMPORTANT_TEST);
)

CHEAT_SKIP(pointless,
	cheat_assert((0 | ~0) == 0);
)

CHEAT_TEST(story,
	puts("Here's a touching story.");
	puts("Once upon a time I ran a test.");
)

CHEAT_TEST(nothing_is_right, {})

CHEAT_TEST(crash,
	fputs(" and crashing.\n", stderr);

	((void (*)(void))NULL)();
)

CHEAT_TEST(the_end,
	puts("The end.");
)

CHEAT_TEST(bye, ;)
