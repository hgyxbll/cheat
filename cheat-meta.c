#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static int cheat_print_parameters(size_t const parameters) {
	size_t index;

	for (index = 0;
			index < parameters;
			++index) {
		if (index != 0)
			(void )printf(", ");
		(void )printf("x%zu", index + 1);
	}
	return 0;
}

static int cheat_print_definitions(size_t const parameters) {
	size_t index;

	for (index = 1;
			index < parameters;
			++index) {
		(void )printf("#define CHEAT_COMMAS_%zu(", index);
		(void )cheat_print_parameters(index + 1);
		(void )printf(") ");
		(void )cheat_print_parameters(index + 1);
		(void )printf("\n");
	}
	return 0;
}

int main(int const count, char** const arguments) {
	long int index;

	if (count != 2)
		return EXIT_FAILURE;

	index = strtol(arguments[1], NULL, 10);
	if (index < 1 || index == LONG_MAX)
		return EXIT_FAILURE;

	(void )cheat_print_definitions((size_t )index);

	return EXIT_SUCCESS;
}
