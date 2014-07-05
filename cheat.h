/*
Copyright (c) 2012, Guillermo "Tordek" Freschi
Copyright (c) 2013, Sampsa "Tuplanolla" Kiiskinen
All rights reserved.

The full license can be found in the LICENSE file.
*/

#ifndef CHEAT_H
#define CHEAT_H

/*
Identifiers starting with
 CHEAT_ and cheat_ are
 reserved for internal use and
 identifiers starting with
 cheat_test_ for external use.
*/

#ifndef __BASE_FILE__
#error "the __BASE_FILE__ preprocessor directive is not defined"
#endif

/*
This disables GNU extensions when
 using compilers that do not support them.
*/
#ifndef __GNUC__
#define __attribute__(_)
#else
#define __io__ __cold__ /* This is informational. */
#endif

#ifdef __cplusplus

extern "C" {

/*
This is used to cast a void pointer to any other pointer type.
*/
#define CHEAT_CAST(type) \
	(type )

#else

#define CHEAT_CAST(type)

#endif

/*
These headers are also
 available externally even though
 they do not need to be.
*/
#include <errno.h> /* errno */
#include <limits.h> /* INT_MAX */
#include <setjmp.h> /* jmp_buf */
#include <signal.h> /* SIGABRT, SIGFPE, SIGILL, SIGSEGV, SIGTERM */
#include <stdarg.h> /* va_list */
#include <stddef.h> /* NULL, size_t */
#include <stdio.h> /* BUFSIZ, FILE, stderr, stdout */
#include <stdlib.h> /* EXIT_FAILURE, EXIT_SUCCESS */
#include <string.h>

#if __STDC_VERSION__ >= 199901L

#include <stdbool.h> /* bool, false, true */
#include <stdint.h> /* SIZE_MAX */

#define CHEAT_SIZE_FORMAT "%zu"

#define CHEAT_CAST_SIZE(size) \
	(size)

#else

#ifndef __cplusplus
typedef int bool;
#define false 0
#define true (!false)
#endif

#ifndef SIZE_MAX
#define SIZE_MAX ((size_t )-1)
#endif

#define CHEAT_SIZE_FORMAT "%lu"

#define CHEAT_CAST_SIZE(size) \
	((unsigned long int )(size))

#endif

#ifdef _WIN32
#include <windows.h> /* spaghetti */
#elif _POSIX_C_SOURCE >= 200112L
#include <sys/types.h> /* pid_t, ssize_t */
#include <sys/wait.h>
#include <unistd.h> /* STDOUT_FILENO */
#endif

/*
These make preprocessor directives work like statements.
*/
#define CHEAT_BEGIN do {
#define CHEAT_END } while (false)

/*
These are ISO/IEC 6429 escape sequences for
 communicating text attributes to terminal emulators.
*/
#define CHEAT_RESET "\x1b[0m"
#define CHEAT_BOLD "\x1b[1m"
#define CHEAT_FOREGROUND_GRAY "\x1b[30;1m"
#define CHEAT_FOREGROUND_RED "\x1b[31;1m"
#define CHEAT_FOREGROUND_GREEN "\x1b[32;1m"
#define CHEAT_FOREGROUND_YELLOW "\x1b[33;1m"
#define CHEAT_FOREGROUND_BLUE "\x1b[34;1m"
#define CHEAT_FOREGROUND_MAGENTA "\x1b[35;1m"
#define CHEAT_FOREGROUND_CYAN "\x1b[36;1m"
#define CHEAT_FOREGROUND_WHITE "\x1b[37;1m"
#define CHEAT_BACKGROUND_BLACK "\x1b[40;1m"
#define CHEAT_BACKGROUND_RED "\x1b[41;1m"
#define CHEAT_BACKGROUND_GREEN "\x1b[42;1m"
#define CHEAT_BACKGROUND_YELLOW "\x1b[43;1m"
#define CHEAT_BACKGROUND_BLUE "\x1b[44;1m"
#define CHEAT_BACKGROUND_MAGENTA "\x1b[45;1m"
#define CHEAT_BACKGROUND_CYAN "\x1b[46;1m"
#define CHEAT_BACKGROUND_GRAY "\x1b[47;1m"

/*
Computes the compiled size of an array (not a pointer) and returns it.
*/
#define CHEAT_SIZE(array) \
	(sizeof array / sizeof *array)

/*
Computes the maximum string length of an unsigned integer type and returns it.
*/
#define CHEAT_LENGTH(type) \
	(CHAR_BIT * sizeof type / 3 + 1) /* This is an upper bound for
		the base 2 logarithm of 10. */

/*
Prints an error message and
 terminates the program.
The error number is context sensitive and
 might only contain the least significant bytes of the actual error code.
*/
#define cheat_death(message, number) \
	CHEAT_BEGIN \
		(void )fprintf(stderr, \
				"%s:%d: %s (0x%x)\n", \
				__FILE__, __LINE__, message, (unsigned int )number); \
		exit(EXIT_FAILURE); \
	CHEAT_END /* Using cheat_print() is intentionally avoided here. */

enum cheat_type {
	CHEAT_TESTER, /* Something to test. */
	CHEAT_UP_SETTER, /* Something to do before tests. */
	CHEAT_DOWN_TEARER, /* Something to do after tests. */
	CHEAT_TERMINATOR /* Nothing to do. */
};

/*
This could be defined as a function type instead of function pointer type, but
 that would not be consistent with the standard library and
 would confuse some old compilers.
*/
typedef void (* cheat_procedure)(void); /* A test or a utility procedure. */

/*
It would not hurt to have
  __attribute__ ((__reorder__))
 on any of these structures since they are only for internal use.
*/

struct cheat_unit {
	char const* name; /* The name to use for
			generating identifiers and
			reporting test results. */

	enum cheat_type const type; /* The type of the procedure. */

	cheat_procedure const procedure; /* The procedure to call. */
};

enum cheat_outcome {
	CHEAT_INDETERMINATE, /* Nothing happened. */
	CHEAT_SUCCESS, /* A success happened. */
	CHEAT_FAILURE, /* A failure happened. */
	CHEAT_CRASHED, /* A critical failure happened. */
	CHEAT_IGNORED /* Anything could have happened. */
};

enum cheat_harness {
	CHEAT_UNSAFE, /* Tests are called without any security measures. */
	CHEAT_SAFE, /* Tests are called in their own subprocesses that
			are monitored. */
	CHEAT_DANGEROUS /* Tests are called and
			suppressing the signals they may raise is attempted. */
};

enum cheat_style {
	CHEAT_PLAIN, /* Messages are printed without decorations. */
	CHEAT_COLORFUL, /* Messages are printed with colors. */
	CHEAT_MINIMAL /* Only numbered summaries are printed. */
};

struct cheat_suite {
	size_t const* unit_count; /* The amount of procedures. */
	struct cheat_unit const* units; /* The procedures. */

	jmp_buf cheat_jmp_buf; /* The recovery point for fatal signals. */

	size_t tests_successful; /* The amount of successful tests so far. */
	size_t tests_failed; /* The amount of failed tests so far. */
	size_t tests_run; /* The amount of tests run or ignored so far. */

	enum cheat_outcome outcome; /* The outcome of
			the most recently completed test. */
	int status; /* The return value of
			the most recently completed test in case it returned or
			an undefined value in case it was aborted. */

	size_t message_count; /* The amount of messages so far. */
	size_t message_capacity; /* The amount of messages until
			more memory has to be allocated. */
	char** messages; /* The messages so far. */

	char* program; /* The program name given to main(). */

	size_t argument_count; /* The amount of arguments for the program. */
	char** arguments; /* The arguments for the program. */

	enum cheat_harness harness; /* The security measures to use. */

	enum cheat_style style; /* The style of printed messages. */

	FILE* captured_stdout; /* The stream subprocesses use as
			their standard output stream. */
};

/* TODO Carry out the plan to make the suite global,
	hopefully allowing for a proper separation of side effects and state. */
static struct cheat_suite cheat_suite;

/*
Suppresses a signal and
 returns to a point before it was raised.
*/
__attribute__ ((__noreturn__))
static void cheat_handle_signal(int const number) {
	longjmp(cheat_suite.cheat_jmp_buf, number);
}

/*
Calculates the arithmetic mean of two sizes and
 returns it.
*/
__attribute__ ((__const__, __warn_unused_result__))
static size_t cheat_mean(size_t const size, size_t const another_size) {
	if (another_size < size)
		return cheat_mean(another_size, size);

	return size + (another_size - size) / 2;
}

/*
Returns a new size that
 has been incremented so that
 reallocation costs are minimized or
 returns the old size unchanged in case it is maximal.
*/
__attribute__ ((__pure__, __warn_unused_result__))
static size_t cheat_expand(size_t const size) {
	if (size < sizeof (int))
		return sizeof (int);

	if (size > SIZE_MAX / 2 + SIZE_MAX / 4)
		return cheat_mean(size, SIZE_MAX);

	return size + size / 2;
}

/*
Finds the amount of conversion specifiers in
 a format string.
Valid specifiers start with '%' and
 are not immediately followed by '%' or '\0'.
*/
__attribute__ ((__nonnull__, __pure__, __warn_unused_result__))
static size_t cheat_format_specifiers(char const* const format) {
	size_t count;
	size_t index;

	count = 0;
	for (index = 0;
			format[index] != '\0';
			++index)
		if (format[index] == '%') {
			if (!(format[index + 1] == '%'))
				++count;
			++index;
		}

	return count;
}

/*
Safely allocates memory for
 a block of size (size + extra_size) and
 returns a pointer to the allocated region or
 returns NULL and sets errno in case of a failure.
*/
__attribute__ ((__malloc__, __warn_unused_result__))
static void* cheat_allocate_more(size_t const size, size_t const extra_size) {
	if (extra_size > SIZE_MAX - size)
		return NULL;

	return malloc(size + extra_size);
}

/*
Safely allocates memory for
 an array of size (count * size) and
 returns a pointer to the allocated region or
 returns NULL and sets errno in case of a failure.
*/
__attribute__ ((__warn_unused_result__))
static void* cheat_allocate_array(size_t const count, size_t const size) {
	if (count > SIZE_MAX / size)
		return NULL;

	return malloc(count * size);
}

/*
Safely reallocates memory for
 an array of size (count * size) and
 returns a pointer to the allocated region or
 returns NULL and sets errno in case of a failure.
*/
__attribute__ ((__warn_unused_result__))
static void* cheat_reallocate_array(void* const pointer,
		size_t const count, size_t const size) {
	if (count > SIZE_MAX / size)
		return NULL;

	return realloc(pointer, count * size);
}

/*
Joins words into a line with spaces in between.
The allocated memory is left for the caller.
*/
__attribute__ ((__malloc__, __nonnull__, __pure__, __warn_unused_result__))
static char* cheat_allocate_joined(char const* const* const words,
		size_t const count) {
	size_t* lengths;
	size_t length;
	char* line;
	size_t index;

	lengths = CHEAT_CAST(size_t*) cheat_allocate_array(count, sizeof *lengths);
	if (lengths == NULL)
		return NULL;
	for (index = 0;
			index < count;
			++index)
		lengths[index] = strlen(words[index]);
	length = 1;
	for (index = 0;
			index < count;
			++index) {
		if (lengths[index] > SIZE_MAX - length - 1) {
			free(lengths);
			return NULL;
		}
		length += lengths[index] + 1;
	}
	line = CHEAT_CAST(char*) malloc(length);
	if (line == NULL) {
		free(lengths);
		return NULL;
	}
	length = 0;
	for (index = 0;
			index < count;
			++index) {
		memcpy(&line[length], words[index], lengths[index]);
		length += lengths[index];
		line[length] = ' ';
		++length;
	}
	line[length - 1] = '\0';
	free(lengths);
	return line;
}

/*
Finds a test by its name and
 returns a pointer to it or
 returns NULL in case no test is found.
*/
__attribute__ ((__nonnull__, __pure__, __warn_unused_result__))
static struct cheat_unit const* cheat_find_test(
		struct cheat_suite const* const suite,
		char const* const name) {
	struct cheat_unit const* unit;
	size_t index;

	for (index = 0;
			index < *suite->unit_count;
			++index) {
		unit = &suite->units[index];
		if (unit->type == CHEAT_TESTER
				&& strcmp(unit->name, name) == 0)
			return unit;
	}
	return NULL;
}

/*
Prints a formatted string into a string or
 fails safely in case the amount of conversion specifiers in
 the format string does not match the expected count.
*/
__attribute__ ((__format__ (__printf__, 1, 5), __pure__, __nonnull__ (1)))
static int cheat_print_string(char const* const format,
		char* const destination,
		size_t const count, size_t const size, ...) {
	va_list list;
	int result;

	if (cheat_format_specifiers(format) != count)
		return -1;

	va_start(list, size);

#ifdef _WIN32
	result = vsprintf_s(destination, size, format, list);
#elif __STDC_VERSION__ >= 199901L
	result = vsnprintf(destination, size, format, list);
#else
	result = vsprintf(destination, format, list);
#endif

	va_end(list);

	return result;
}

/*
Prints a formatted string to a stream or
 fails safely in case the amount of conversion specifiers in
 the format string does not match the expected count.
*/
__attribute__ ((__format__ (__printf__, 1, 4), __io__, __nonnull__ (1)))
static int cheat_print(char const* const format,
		FILE* const stream,
		size_t const count, ...) {
	va_list list;
	int result;

	if (cheat_format_specifiers(format) != count)
		return -1;

	va_start(list, count);
	result = vfprintf(stream, format, list);
	va_end(list);

	return result;
}

/*
Clears a test suite.
The memory allocated for the message queue and
 the messages themselves is released, but
 everything else is left for the caller.
*/
__attribute__ ((__nonnull__))
static void cheat_clear(struct cheat_suite* const suite) {
	suite->tests_successful = 0;
	suite->tests_failed = 0;
	suite->tests_run = 0;
	suite->outcome = CHEAT_INDETERMINATE;
	if (suite->messages != NULL) {
		while (suite->message_count > 0)
			free(suite->messages[--suite->message_count]); /* Memory B. */
		suite->message_capacity = 0;
		free(suite->messages); /* Memory A. */
		suite->messages = NULL;
	}
	suite->program = NULL;
	suite->argument_count = 0;
	suite->arguments = NULL;
	suite->harness = CHEAT_UNSAFE;
	suite->style = CHEAT_PLAIN;
	suite->captured_stdout = stdout;
}

/*
Initializes an undefined test suite.
*/
__attribute__ ((__nonnull__))
static void cheat_initialize(struct cheat_suite* const suite) {
	suite->message_count = 0;
	suite->message_capacity = 0;
	suite->messages = NULL;
	cheat_clear(suite);
}

/*
Adds a message in
 the form of a byte buffer to
 the message queue of a test suite.
*/
__attribute__ ((__nonnull__))
static void cheat_append_message(struct cheat_suite* const suite,
		unsigned char const* const data, size_t const size) {
	size_t message_capacity;
	size_t message_count;
	char** messages;
	char* message;

	if (size == 0)
		return;

	if (suite->message_count == SIZE_MAX)
		cheat_death("too many messages", suite->message_count);
	message_count = suite->message_count + 1;

	message = CHEAT_CAST(char*) cheat_allocate_more(size, 1); /* Memory B. */
	if (message == NULL)
		cheat_death("failed to allocate memory for a new message", errno);
	memcpy(message, data, size);
	message[size] = '\0';
	/* message[0] = '|'; */ /* This showcases the wrong! */

	if (suite->message_count == suite->message_capacity) {
		message_capacity = cheat_expand(suite->message_capacity);
		if (message_capacity == suite->message_capacity)
			cheat_death("message capacity exceeded", suite->message_capacity);
		messages = CHEAT_CAST(char**) cheat_reallocate_array(suite->messages,
				message_capacity, sizeof *suite->messages); /* Memory A. */
		if (messages == NULL)
			cheat_death("failed to allocate more memory for messages", errno);
		suite->message_capacity = message_capacity;
		suite->messages = messages;
	}
	suite->messages[suite->message_count] = message;

	suite->message_count = message_count;
}

/*
Adds the outcome of a single test to a suite or
 terminates the program in case of an error.
*/
__attribute__ ((__nonnull__))
static void cheat_handle_outcome(struct cheat_suite* const suite) {
	++suite->tests_run;
	switch (suite->outcome) {
	case CHEAT_SUCCESS:
		++suite->tests_successful;
		break;
	case CHEAT_FAILURE:
		++suite->tests_failed;
		break;
	case CHEAT_IGNORED:
		break;
	case CHEAT_CRASHED:
		++suite->tests_failed;
		break;
	case CHEAT_INDETERMINATE: /* TODO Remove. */
		break;
	default:
		cheat_death("invalid outcome", suite->outcome);
	}
}

/*
Prints a usage summary.
*/
__attribute__ ((__io__, __nonnull__))
static void cheat_print_usage(struct cheat_suite const* const suite) {
	(void )fputs("Usage: ", suite->captured_stdout);
	(void )fputs(suite->program, suite->captured_stdout);
	(void )fputs(" [option] [another option] [...]\n", suite->captured_stdout);
	(void )fputs("\
Options: -c  --colorful   Use ISO/IEC 6429 escape codes to color reports\n\
         -d  --dangerous  Pretend that crashing tests do\n\
                          not cause undefined behavior\n\
         -h  --help       Show this help\n\
         -l  --list       List test cases\n\
", suite->captured_stdout); /* String literals must fit into 509 bytes. */
	(void )fputs("\
         -m  --minimal    Only report the amounts of successes, failures\n\
                          and tests run in a machine readable format\n\
         -p  --plain      Present reports in plain text\n\
         -s  --safe       Run tests in isolated subprocesses\n\
         -u  --unsafe     Let crashing tests bring down the whole suite\n\
", suite->captured_stdout);
}

/*
Prints a list of tests.
*/
__attribute__ ((__io__, __nonnull__))
static void cheat_print_tests(struct cheat_suite const* const suite) {
	size_t index;
	bool first;

	first = true;
	for (index = 0;
			index < *suite->unit_count;
			++index) {
		struct cheat_unit const* unit;

		unit = &suite->units[index];
		if (unit->type == CHEAT_TESTER) {
			if (first) {
				(void )fputs("Tests: ", suite->captured_stdout);
				first = false;
			} else
				(void )fputs("       ", suite->captured_stdout);
			(void )fputs(unit->name, suite->captured_stdout);
			(void )fputc('\n', suite->captured_stdout);
		}
	}
}

/*
Prints the outcome of a single test or
 terminates the program in case of an error.
*/
__attribute__ ((__io__, __nonnull__))
static void cheat_print_outcome(struct cheat_suite* const suite) {
	bool print_bar;
	char const* success;
	char const* failure;
	char const* ignored;
	char const* crashed;

	switch (suite->style) {
	case CHEAT_PLAIN:
		print_bar = true;
		success = ".";
		failure = ":";
		ignored = "?";
		crashed = "!";
		break;
	case CHEAT_COLORFUL:
		print_bar = true;
		success = CHEAT_BACKGROUND_GREEN
			"." CHEAT_RESET;
		failure = CHEAT_BACKGROUND_RED
			":" CHEAT_RESET;
		ignored = CHEAT_BACKGROUND_YELLOW
			"?" CHEAT_RESET;
		crashed = CHEAT_BACKGROUND_RED
			"!" CHEAT_RESET;
		break;
	case CHEAT_MINIMAL:
		print_bar = false;
		break;
	default:
		cheat_death("invalid style", suite->style);
	}

	if (print_bar) {
		switch (suite->outcome) {
		case CHEAT_SUCCESS:
			(void )fputs(success, suite->captured_stdout);
			break;
		case CHEAT_FAILURE:
			(void )fputs(failure, suite->captured_stdout);
			break;
		case CHEAT_IGNORED:
			(void )fputs(ignored, suite->captured_stdout);
			break;
		case CHEAT_CRASHED:
			(void )fputs(crashed, suite->captured_stdout);
			break;
		case CHEAT_INDETERMINATE: /* TODO Remove. */
			break;
		default:
			cheat_death("invalid outcome", suite->outcome);
		}
		(void )fflush(suite->captured_stdout);
	}
}

/*
Prints a summary of all tests or
 terminates the program in case of an error.
*/
__attribute__ ((__io__, __nonnull__))
static void cheat_print_summary(struct cheat_suite* const suite) {
	bool any_successes;
	bool any_failures;
	bool any_run;
	bool print_messages;
	bool print_summary;
	bool print_conclusion;
	bool print_zero;
	char const* separator_string;
	char const* successful_format;
	char const* and_string;
	char const* failed_format;
	char const* of_string;
	char const* run_format;
	char const* conclusion_string;

	any_successes = suite->tests_successful != 0;
	any_failures = suite->tests_failed != 0;
	any_run = suite->tests_run != 0;
	switch (suite->style) {
	case CHEAT_PLAIN:
		print_messages = true;
		print_summary = true;
		print_conclusion = true;
		print_zero = false;
		separator_string = "---";
		successful_format = CHEAT_SIZE_FORMAT " successful";
		and_string = " and ";
		failed_format = CHEAT_SIZE_FORMAT " failed";
		of_string = " of ";
		run_format = CHEAT_SIZE_FORMAT " run";
		if (!any_failures)
			conclusion_string = "SUCCESS";
		else
			conclusion_string = "FAILURE";
		break;
	case CHEAT_COLORFUL:
		print_messages = true;
		print_summary = true;
		print_conclusion = true;
		print_zero = false;
		separator_string = CHEAT_FOREGROUND_GRAY
			"---" CHEAT_RESET;
		successful_format = CHEAT_FOREGROUND_GREEN
			CHEAT_SIZE_FORMAT " successful" CHEAT_RESET;
		and_string = " and ";
		failed_format = CHEAT_FOREGROUND_RED
			CHEAT_SIZE_FORMAT " failed" CHEAT_RESET;
		of_string = " of ";
		run_format = CHEAT_FOREGROUND_YELLOW
			CHEAT_SIZE_FORMAT " run" CHEAT_RESET;
		if (!any_failures)
			conclusion_string = CHEAT_FOREGROUND_GREEN
				"SUCCESS" CHEAT_RESET;
		else
			conclusion_string = CHEAT_FOREGROUND_RED
				"FAILURE" CHEAT_RESET;
		break;
	case CHEAT_MINIMAL:
		print_messages = false;
		print_summary = true;
		print_conclusion = false;
		print_zero = true;
		successful_format = CHEAT_SIZE_FORMAT;
		and_string = " ";
		failed_format = CHEAT_SIZE_FORMAT;
		of_string = " ";
		run_format = CHEAT_SIZE_FORMAT;
		break;
	default:
		cheat_death("invalid style", suite->style);
	}

	if (print_messages && any_run) {
		(void )fputc('\n', suite->captured_stdout);
		if (suite->messages != NULL) {
			size_t index;

			(void )fputs(separator_string, suite->captured_stdout);
			(void )fputc('\n', suite->captured_stdout);
			for (index = 0;
					index < suite->message_count;
					++index)
				(void )fputs(suite->messages[index], suite->captured_stdout);
		}
		(void )fputs(separator_string, suite->captured_stdout);
		(void )fputc('\n', suite->captured_stdout);
	}
	if (print_summary) {
		if (print_zero || any_successes)
			(void )cheat_print(successful_format, suite->captured_stdout,
					1, CHEAT_CAST_SIZE(suite->tests_successful));
		if (print_zero || (any_successes && any_failures))
			(void )fputs(and_string, suite->captured_stdout);
		if (print_zero || any_failures)
			(void )cheat_print(failed_format, suite->captured_stdout,
					1, CHEAT_CAST_SIZE(suite->tests_failed));
		if (print_zero || (any_successes || any_failures))
			(void )fputs(of_string, suite->captured_stdout);
		(void )cheat_print(run_format, suite->captured_stdout,
				1, CHEAT_CAST_SIZE(suite->tests_run));
		(void )fputc('\n', suite->captured_stdout);
	}
	if (print_conclusion) {
		(void )fputs(conclusion_string, suite->captured_stdout);
		(void )fputc('\n', suite->captured_stdout);
	}
}

/*
Prints an error message.
*/
__attribute__ ((__io__, __nonnull__))
static void cheat_print_failure(struct cheat_suite* const suite,
		char const* const expression,
		char const* const file, size_t const line) {
	bool print_assertion;
	char const* assertion_format;

	switch (suite->style) {
	case CHEAT_COLORFUL:
		print_assertion = true;
		assertion_format = CHEAT_BOLD "%s:" CHEAT_SIZE_FORMAT ":"
			CHEAT_RESET " assertion failed: '"
			CHEAT_BOLD "%s"
			CHEAT_RESET "'\n";
		break;
	case CHEAT_PLAIN:
		print_assertion = true;
		assertion_format = "%s:" CHEAT_SIZE_FORMAT ": assertion failed: '%s'\n";
		break;
	case CHEAT_MINIMAL:
		print_assertion = false;
		break;
	default:
		cheat_death("invalid style", suite->style);
	}

	if (print_assertion) {
		if (suite->harness == CHEAT_SAFE)
			(void )cheat_print(assertion_format, suite->captured_stdout,
					3, file, line, expression);
		else {
			size_t size;
			char* buffer;

			size = strlen(assertion_format)
				+ strlen(file)
				+ CHEAT_LENGTH(line)
				+ strlen(expression); /* TODO Check overflow. */
			buffer = CHEAT_CAST(char*) malloc(size);
			if (buffer == NULL)
				cheat_death("failed to allocate memory", errno);
			(void )cheat_print_string(assertion_format, buffer,
					3, size, file, CHEAT_CAST_SIZE(line), expression);

			cheat_append_message(suite, (unsigned char* )buffer, size);

			free(buffer);
		}
	}
}

/*
Checks a single assertion and
 prints an error message if it fails.
*/
__attribute__ ((__io__, __nonnull__))
static void cheat_check(struct cheat_suite* const suite,
		bool const result,
		char const* const expression,
		char const* const file, size_t const line) {
	if (!result) {
		suite->outcome = CHEAT_FAILURE;
		cheat_print_failure(suite, expression, file, line);
	}
}

/*
Runs all utility procedures of a certain type.
*/
__attribute__ ((__io__))
static void cheat_run_utilities(struct cheat_suite* const suite,
		enum cheat_type const type) {
	size_t index;

	for (index = 0;
			index < *suite->unit_count;
			++index)
		if (suite->units[index].type == type)
			(suite->units[index].procedure)();
}

/*
Runs the core of a test or
 terminates the program in case of an error.
*/
__attribute__ ((__io__, __nonnull__))
static void cheat_run_core(struct cheat_unit const* const unit) {
	if (unit->type == CHEAT_TESTER)
		(unit->procedure)();
	else
		cheat_death("not a test", unit->type);
}

/*
Runs a test from
 a test suite.
*/
__attribute__ ((__io__, __nonnull__, __warn_unused_result__))
static enum cheat_outcome cheat_run_test(struct cheat_suite* const suite,
		struct cheat_unit const* const unit) {
	suite->outcome = CHEAT_SUCCESS;

	cheat_run_utilities(suite, CHEAT_UP_SETTER);
	cheat_run_core(unit);
	cheat_run_utilities(suite, CHEAT_DOWN_TEARER);

	return suite->outcome;
}

/*
Creates a subprocess and
 runs a test in it.
*/
__attribute__ ((__io__, __nonnull__))
static void cheat_run_isolated_test(struct cheat_suite* const suite,
		struct cheat_unit const* const test) {

#ifdef _WIN32

	HANDLE reader;
	HANDLE writer;
	SECURITY_ATTRIBUTES security;
	STARTUPINFO startup;
	PROCESS_INFORMATION process;
	SIZE_T size;
	DWORD error;
	PCHAR name; /* TODO Check type conversions. */
	PCHAR* arguments;
	PCHAR command;
	DWORD status;

	/*
	The memory behind the structures is zeroed to
	 avoid compatibility issues if their fields change.
	*/
	ZeroMemory(&security, sizeof security);
	ZeroMemory(&startup, sizeof startup);
	ZeroMemory(&process, sizeof process);

	security.nLength = sizeof security;
	security.lpSecurityDescriptor = NULL;
	security.bInheritHandle = TRUE;

	if (!CreatePipe(&reader, &writer, &security, 0))
		cheat_death("failed to create a pipe", GetLastError());

	startup.cb = sizeof startup;
	startup.lpReserved = NULL;
	startup.lpDesktop = NULL;
	startup.lpTitle = NULL;
	startup.dwFlags = STARTF_USESTDHANDLES;
	startup.cbReserved2 = 0;
	startup.lpReserved2 = NULL;
	startup.hStdInput = NULL;
	startup.hStdOutput = writer;
	startup.hStdError = NULL; /* TODO Consider capturing. */

	size = strlen(test->name) + 1;
	name = malloc(size);
	if (name == NULL)
		cheat_death("failed to allocate memory for the name of a test", errno);
	memcpy(name, test->name, size);

	if (suite->argument_count > SIZE_MAX - 3)
		cheat_death("too many arguments", suite->argument_count);
	arguments = cheat_allocate_array(suite->argument_count + 3,
			sizeof *suite->arguments);
	if (arguments == NULL)
		cheat_death("failed to allocate memory for an argument list", errno);
	arguments[0] = suite->program;
	arguments[1] = name;
	memcpy(&arguments[2], suite->arguments,
			suite->argument_count * sizeof *suite->arguments);
	arguments[suite->argument_count + 2] = NULL;

	command = cheat_allocate_joined(arguments, suite->argument_count + 2);
	error = errno;
	free(name);
	free(arguments);
	if (command == NULL)
		cheat_death("failed to allocate memory for a command", error);
	/* TODO Implicit file extensions and Unicode characters ruin this. */
	if (!CreateProcess(suite->program, command, NULL, NULL,
				TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL,
				&startup, &process)) {
		error = GetLastError();
		free(command);
		cheat_death("failed to create a process", error);
	}
	free(command);

	if (!CloseHandle(writer))
		cheat_death("failed to close the write end of a pipe", GetLastError());

	do {
		UCHAR buffer[BUFSIZ];
		DWORD size;

		/* TODO This always fails or something. */
		if (!ReadFile(reader, buffer, sizeof buffer, &size, NULL))
			break;
		cheat_append_message(suite, buffer, (size_t )size); /* This is wrong! */
	} while (TRUE);

	if (!CloseHandle(reader))
		cheat_death("failed to close the read end of a pipe", GetLastError());

	if (WaitForSingleObject(process.hProcess, INFINITE) == WAIT_FAILED)
		cheat_death("failed to wait for a process", GetLastError());

	if (!GetExitCodeProcess(process.hProcess, &status))
		cheat_death("failed to retrieve the exit status of a process",
				GetLastError());

	if (!CloseHandle(process.hProcess))
		cheat_death("failed to close a process handle", GetLastError());
	if (!CloseHandle(process.hThread))
		cheat_death("failed to close a thread handle", GetLastError());

	/* TODO This is sometimes bogus; only 0 is guaranteed to mean successful. */
	/* printf(" [%s -> %d]\n", test->name, status); */
	suite->status = status;
	suite->outcome = status == 3 ? CHEAT_CRASHED : status;

#elif _POSIX_C_SOURCE >= 200112L

	pid_t pid;
	int fds[2];
	int reader;
	int writer;

	if (pipe(fds) == -1)
		cheat_death("failed to create a pipe", errno);
	reader = fds[0];
	writer = fds[1];

	pid = fork();
	if (pid == -1)
		cheat_death("failed to create a process", errno);
	else if (pid == 0) {
		enum cheat_outcome outcome;

		if (close(reader) == -1)
			cheat_death("failed to close the read end of a pipe", errno);
		if (dup2(writer, STDOUT_FILENO) == -1)
			cheat_death("failed to redirect standard output", errno);
		outcome = cheat_run_test(suite, test);
		if (close(writer) == -1)
			cheat_death("failed to close the write end of a pipe", errno);
		exit(outcome);
	} else {
		int status;

		if (close(writer) == -1)
			cheat_death("failed to close the write end of a pipe", errno);
		do {
			unsigned char buffer[BUFSIZ];
			ssize_t size;

			/* TODO Outcome, not status. */
			/*
			Okay, here's the plan, at least approximately so.
			Message splitting has still its share of problems.
			If the buffer contains a double '\0' before the end, then
			 the message ends at the terminator as usual, but
			 the outcome comes after it.
			The size of the exit status variable depends on
			 how many bytes are left over; while
			 some assumptions about its size have to be made,
			 it is not fixed between implementations.
			Least significant bytes come first.
			*/
			size = read(reader, buffer, sizeof buffer);
			if (size == -1)
				cheat_death("failed to read from a pipe", errno);
			if (size == 0)
				break;
			cheat_append_message(suite, buffer, (size_t )size); /* Wrong! */
		} while (true);
		if (waitpid(pid, &status, 0) == -1)
			cheat_death("failed to wait for a process", errno);
		if (close(reader) == -1)
			cheat_death("failed to close the read end of a pipe", errno);
		/* TODO The outcome should be sent through the pipe instead. */
		/* printf(" [%s -> %d]\n", test->name, status); */
		if (WIFEXITED(status)) {
			suite->outcome = (enum cheat_outcome )WEXITSTATUS(status);
			suite->status = WEXITSTATUS(status);
		} else
			suite->outcome = CHEAT_CRASHED;
	}

#else

	cheat_death("failed to isolate a test", 0);

#endif

}

/*
Parses the arguments of a test suite and
 decides whether to do work or complain.
*/
__attribute__ ((__io__, __nonnull__))
static void cheat_parse(struct cheat_suite* const suite) {
	bool colorful;
	bool dangerous;
	bool help;
	bool list;
	bool minimal;
	bool plain;
	bool safe;
	bool unsafe;
	size_t names;
	char const* name;
	bool test;
	bool options;
	size_t index;

	colorful = false;
	dangerous = false;
	help = false;
	list = false;
	minimal = false;
	plain = false;
	safe = false;
	unsafe = false;
	names = 0;
	options = true;
	for (index = 0;
			index < suite->argument_count;
			++index) {
		if (options && suite->arguments[index][0] == '-') {
			if (strcmp(suite->arguments[index], "--") == 0)
				options = false;
			else if (strcmp(suite->arguments[index], "-c") == 0
					|| strcmp(suite->arguments[index], "--colorful") == 0)
				colorful = true;
			else if (strcmp(suite->arguments[index], "-d") == 0
					|| strcmp(suite->arguments[index], "--dangerous") == 0)
				dangerous = true;
			else if (strcmp(suite->arguments[index], "-h") == 0
					|| strcmp(suite->arguments[index], "--help") == 0)
				help = true;
			else if (strcmp(suite->arguments[index], "-l") == 0
					|| strcmp(suite->arguments[index], "--list") == 0)
				list = true;
			else if (strcmp(suite->arguments[index], "-m") == 0
					|| strcmp(suite->arguments[index], "--minimal") == 0)
				minimal = true;
			else if (strcmp(suite->arguments[index], "-p") == 0
					|| strcmp(suite->arguments[index], "--plain") == 0)
				plain = true;
			else if (strcmp(suite->arguments[index], "-s") == 0
					|| strcmp(suite->arguments[index], "--safe") == 0)
				safe = true;
			else if (strcmp(suite->arguments[index], "-u") == 0
					|| strcmp(suite->arguments[index], "--unsafe") == 0)
				unsafe = true;
			else
				cheat_death("invalid option", index);
		} else {
			++names;
			name = suite->arguments[index];
		}
	}
	test = names != 0;

	if (help) { /* TODO This logic. */
		cheat_print_usage(suite);
	} else if (list) {
		cheat_print_tests(suite);
	} else if (!(colorful && minimal) && !help) {
		if (plain)
			suite->style = CHEAT_PLAIN;
		if (colorful)
			suite->style = CHEAT_COLORFUL;
		if (minimal)
			suite->style = CHEAT_MINIMAL;
		if (safe)
			suite->harness = CHEAT_SAFE;
		if (unsafe)
			suite->harness = CHEAT_UNSAFE;
		if (dangerous)
			suite->harness = CHEAT_DANGEROUS;
		if (test) {
			struct cheat_unit const* unit;
			enum cheat_outcome outcome; /* TODO Write and use. */

			unit = cheat_find_test(suite, name);
			if (unit == NULL)
				cheat_death("test not found", 0);
			outcome = cheat_run_test(suite, unit);
			/* TODO Outcome, not status. */
			exit(outcome);
		} else {
			if (suite->harness == CHEAT_DANGEROUS) {
				if (signal(SIGABRT, cheat_handle_signal) == SIG_ERR)
					cheat_death("failed to disable the ABRT signal", errno);
				if (signal(SIGFPE, cheat_handle_signal) == SIG_ERR)
					cheat_death("failed to disable the FPE signal", errno);
				if (signal(SIGILL, cheat_handle_signal) == SIG_ERR)
					cheat_death("failed to disable the ILL signal", errno);
				if (signal(SIGSEGV, cheat_handle_signal) == SIG_ERR)
					cheat_death("failed to disable the SEGV signal", errno);
				if (signal(SIGTERM, cheat_handle_signal) == SIG_ERR)
					cheat_death("failed to disable the TERM signal", errno);
			}
			for (index = 0;
					index < *suite->unit_count;
					++index) {
				struct cheat_unit const* unit;
				enum cheat_outcome outcome; /* TODO Read and use. */

				unit = &suite->units[index];
				if (unit->type != CHEAT_TESTER)
					continue;
				if (suite->harness == CHEAT_SAFE)
					cheat_run_isolated_test(suite, unit);
				else if (suite->harness == CHEAT_DANGEROUS) {
					if (setjmp(suite->cheat_jmp_buf) == 0)
						outcome = cheat_run_test(suite, unit);
					else
						suite->outcome = CHEAT_CRASHED;
				} else
					outcome = cheat_run_test(suite, unit);
				cheat_handle_outcome(suite);
				cheat_print_outcome(suite);
			}
			cheat_print_summary(suite);
		}
	} else
		cheat_death("invalid combination of options", 0);
}

#define CHEAT_PASS 1 /* This is informational. */

/*
Some of the symbols used here are defined in the third pass.
*/

#define CHEAT_TEST(name, body) \
	static void cheat_test_##name(void);

#define CHEAT_SET_UP(body) \
	static void cheat_set_up(void);

#define CHEAT_TEAR_DOWN(body) \
	static void cheat_tear_down(void);

#define CHEAT_DECLARE(body)

#include __BASE_FILE__

#undef CHEAT_TEST
#undef CHEAT_SET_UP
#undef CHEAT_TEAR_DOWN
#undef CHEAT_DECLARE

#undef CHEAT_PASS

#define CHEAT_PASS 2

#define CHEAT_TEST(name, body) \
	{ \
		#name, \
		CHEAT_TESTER, \
		cheat_test_##name \
	},

#define CHEAT_SET_UP(body) \
	{ \
		NULL, \
		CHEAT_UP_SETTER, \
		cheat_set_up \
	},

#define CHEAT_TEAR_DOWN(body) \
	{ \
		NULL, \
		CHEAT_DOWN_TEARER, \
		cheat_tear_down \
	},

#define CHEAT_DECLARE(body)

static struct cheat_unit const cheat_units[] = {
#include __BASE_FILE__
	{
		NULL,
		CHEAT_TERMINATOR,
		NULL
	} /* This terminator only exists to avoid
		the problems some compilers have with
		trailing commas or arrays with zero size. */
};

/* TODO Less verbosity. */
static size_t const cheat_unit_count = CHEAT_SIZE(cheat_units) - 1;

#undef CHEAT_TEST
#undef CHEAT_SET_UP
#undef CHEAT_TEAR_DOWN
#undef CHEAT_DECLARE

#undef CHEAT_PASS

#define CHEAT_PASS 3

/*
Some of the symbols defined here are used in the first pass.
*/

#define CHEAT_TEST(name, body) \
	static void cheat_test_##name(void) body

#define CHEAT_SET_UP(body) \
	static void cheat_set_up(void) body

#define CHEAT_TEAR_DOWN(body) \
	static void cheat_tear_down(void) body

#define CHEAT_DECLARE(body) \
	body

#define cheat_assert(expression) \
	cheat_check(&cheat_suite, expression, #expression, __FILE__, __LINE__)

/*
The third pass continues past the end of this file, but
 the definitions for it end here.
*/

/*
Runs tests from the main test suite and
 returns EXIT_SUCCESS in case all tests passed or
 EXIT_FAILURE in case of an error.
*/
int main(int const count, char** const arguments) {
	struct cheat_suite* suite;
	size_t result;

	/* TODO Wrangle this around. */
	suite = &cheat_suite;
	suite->unit_count = &cheat_unit_count;
	suite->units = cheat_units;

	cheat_initialize(suite);
	suite->program = arguments[0];
	suite->argument_count = (size_t )(count - 1);
	suite->arguments = &arguments[1];
	suite->harness = CHEAT_SAFE;
	suite->style = CHEAT_PLAIN;
	cheat_parse(suite);
	result = suite->tests_failed;
	cheat_clear(suite);
	if (result == 0)
		return EXIT_SUCCESS;
	return EXIT_FAILURE;
}

#ifdef __cplusplus
}
#endif

#endif
