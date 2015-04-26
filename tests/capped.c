#define CHEAT_OUTPUT_CAP 10
#define CHEAT_ERROR_CAP 16
#define CHEAT_SCAN_TYPE char

#include <cheat.h>
#include <stdio.h>

CHEAT_TEST(print,
	cheat_assert(fputs("Let us (define (f x) (+ x 2)).\n", stdout) != EOF
			&& fputs("We see that (= (f 2) 5) is #f.\n", stderr) != EOF);
)

CHEAT_DECLARE(
	static char f(cheat_handle* const handle) {
		int character;

		while ((character = cheat_advancing_read(handle)) != CHEAT_EOF)
			if ((char )character == '(') {
				if ((character = cheat_advancing_read(handle)) != CHEAT_EOF)
					return (char )character;
				else
					cheat_rewind(handle);
			}

		return '\0';
	}
)

CHEAT_TEST(success,
	cheat_assert(cheat_scan_outputs(f) == '+');
	cheat_assert(cheat_scan_errors(f) == 'f');
)

CHEAT_TEST(failure,
	cheat_assert(cheat_scan_outputs(f) == 'd');
	cheat_assert(cheat_scan_errors(f) == '=');
)
