#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <cheat.h>
#include <cheat-helpers.h>
#include <stdio.h>
#include <string.h>

CHEAT_DECLARE(
	char* tmp_string;
)

CHEAT_SET_UP({
	tmp_string = (char* )calloc(1, 50); /* This is stupid. */
})

CHEAT_TEAR_DOWN({
	free(tmp_string);
})


CHEAT_TEST(maths_still_work, {
	cheat_assert(4 == 2+2);
})

CHEAT_TEST(strcat_makes_sense, {
	strcpy(tmp_string, "Hello, ");
	strcat(tmp_string, "World!");

	cheat_assert(0 == strcmp(tmp_string, "Hello, World!"));
})

CHEAT_TEST(failure, {
	cheat_assert(0);
})

CHEAT_TEST(second_failure, {
	cheat_assert(1 == 0);
})

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
