#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <cheat.h>
/* #include <cheat-helpers.h> */
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
		return x * y;
	}

	static bool g(double const x, double const y, double const e) {
		double d;

		if (x < y)
			d = y - x;
		else
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
	size_t i;
	double s;
	double x;

	x0 = 1;
	y0 = 2;
	n = 100;
	xn = 3;
	dx = xn - x0;
	y = y0;
	for (i = 0;
			i < n;
			++i) {
		s = dx / (double )n;
		x = x0 + s * (double )i;
		y += s * f(x, y);
	}

	cheat_assert(g(y, 100, 10));
)

CHEAT_DECLARE(
	size_t size;
	static char* heap;
)

CHEAT_SET_UP(
	char const* stack;

	stack = "string";
	size = strlen(stack) + 1;
	heap = CHEAT_CAST(char*) malloc(size);
	memcpy(heap, stack, size);
)

CHEAT_TEAR_DOWN(
	free(heap);
)


CHEAT_TEST(success,
	fputs("Test began.\n", stderr);

	fputs("Test!\n", stdout);

	fputs("Test ended.\n", stderr);
)

CHEAT_TEST(failure,
	fputs("Test began.\n", stderr);

	fputs("Test!\n", stdout);
	cheat_assert(false);

	fputs("Test ended.\n", stderr);
)


CHEAT_TEST(philosophy_never_worked,
	char const* stack;

	stack = "string";
	cheat_assert(heap == stack);
	cheat_assert(strcmp(heap, stack) == 0);
)

CHEAT_TEST(nothing_is_right, {})

CHEAT_TEST(nothing_is_wrong, {})

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

#ifndef OXYGEN_MOLECULE
#define OXYGEN_MOLECULE (0 == 0)
#endif

CHEAT_TEST(chemistry_is_strange,
	cheat_assert(OXYGEN_MOLECULE);
)

CHEAT_TEST(crash,
	((void (*)(void))NULL)();
)

CHEAT_TEST(punctuation_makes_a_difference, ;)

/*
CHEAT_TEST_WITH_CAPTURED_STDOUT(output_capture, {
	printf("Something stupid");
	cheat_assert(cheat_stream_contains(stdout, "Something"));
})

CHEAT_TEST_WITH_CAPTURED_STDOUT(large_output_capture, {
	printf("%1000s", "Potato");

	cheat_assert(cheat_stream_contains(stdout, "Potato"));
})

CHEAT_TEST_WITH_CAPTURED_STDERR(stderr_capture, {
	fprintf(stderr, "You can also capture errors!");
	cheat_assert(cheat_stream_contains(stderr, "errors"));
})

CHEAT_TEST(segfault, {
	int* foo = NULL;
	printf("%d", *foo);
	cheat_assert(0);
})
*/
