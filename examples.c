#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <cheat.h>
#include <cheats.h>
#include <string.h>

CHEAT_TEST(assertions_are_unclear,
	char const string[] = "yes";
	char character = '*';
	int number = 13;

	cheat_assert(strcmp(string, "no") == 0);
	cheat_assert(character == number);
)

CHEAT_TEST(additional_features_help,
	char const string[] = "yes";
	char character = '*';
	int number = 13;

	cheat_assert_string(string, "no");
	cheat_assert_int(character, number);
)
