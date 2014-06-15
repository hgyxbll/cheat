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
#error "The __BASE_FILE__ symbol is not defined. See the README file for help."
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

/*
These headers are also
 available externally even though
 they do not need to be.
*/
#include <limits.h> /* INT_MAX */
#include <stdarg.h> /* va_list */
#include <stddef.h> /* NULL, size_t */
#include <stdio.h> /* BUFSIZ, FILE, stderr, stdout */
#include <stdlib.h> /* EXIT_FAILURE, EXIT_SUCCESS */
#include <string.h>
#if __STDC_VERSION__ >= 199901L
#include <stdbool.h> /* bool, false, true */
#include <stdint.h> /* SIZE_MAX */
#else
typedef int bool;
#define false 0
#define true 1
#define SIZE_MAX ((size_t )-1)
#endif
#if _POSIX_C_SOURCE >= 200112L
#include <unistd.h> /* STDOUT_FILENO */
#include <sys/types.h> /* pid_t, ssize_t */
#include <sys/wait.h>
#elif defined _WIN32
#include <windows.h> /* spaghetti */
#endif

enum cheat_outcome {
	CHEAT_INDETERMINATE, /* Nothing happened. */
	CHEAT_SUCCESS, /* A success happened. */
	CHEAT_FAILURE, /* A failure happened. */
	CHEAT_IGNORED, /* Anything could have happened. */
	CHEAT_CRASHED /* A segmentation fault happened. */
};

enum cheat_harness {
	CHEAT_CALL, /* Tests are called without any security measures. */
	CHEAT_SIGNAL_HANDLER, /* Tests are called and
			handling the signals they raise is attempted. */
	CHEAT_FORK /* Tests are called in their own subprocesses that
			are monitored. */
};

enum cheat_style {
	CHEAT_PLAIN, /* Messages are printed without decorations. */
	CHEAT_COLORFUL, /* Messages are printed with colors. */
	CHEAT_MINIMAL /* Only numbered summaries are printed. */
};

struct cheat_suite {
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

	char const* program; /* The program name given to main(). */

	enum cheat_harness harness; /* The security measures to use. */

	enum cheat_style style; /* The style of printed messages. */

	FILE* captured_stdout; /* TODO Document. */
};

enum cheat_type {
	CHEAT_TESTER, /* Something to test. */
	CHEAT_UP_SETTER, /* Something to do before tests. */
	CHEAT_DOWN_TEARER /* Something to do after tests. */
};

typedef void (cheat_procedure)(void); /* An untyped procedure. */

typedef void (cheat_test)(struct cheat_suite*); /* A test procedure. */

typedef void (cheat_utility)(void); /* A utility procedure. */

struct cheat_procedure {
	char const* name; /* The name to use for
			generating identifiers and
			reporting test results. */

	enum cheat_type type; /* The type of the procedure. */

	cheat_procedure* procedure; /* The procedure to call would be
			a union of cheat_test and cheat_utility, but
			initializing such a thing would be impossible if
			it was qualified const. */
};

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
#define CHEAT_ARRAY_SIZE(array) \
		(sizeof array / sizeof *array)

#define CHEAT_PASS 1 /* This is informational. */

/*
Some of the symbols used here are defined in the third pass.
*/

#define CHEAT_TEST(name, body) \
		static void cheat_test_##name(struct cheat_suite*);
#define CHEAT_SET_UP(body) \
		static void cheat_set_up(void);
#define CHEAT_TEAR_DOWN(body) \
		static void cheat_tear_down(void);
#define CHEAT_DECLARE(body)

#define CHEAT_TEST_IGNORE(name, body) \
		CHEAT_TEST(name, { \
			cheat_suite->outcome = CHEAT_IGNORED; \
		})

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
			(cheat_procedure* )cheat_test_##name \
		},
#define CHEAT_SET_UP(body) \
		{ \
			NULL, \
			CHEAT_UP_SETTER, \
			(cheat_procedure* )cheat_set_up \
		},
#define CHEAT_TEAR_DOWN(body) \
		{ \
			NULL, \
			CHEAT_DOWN_TEARER, \
			(cheat_procedure* )cheat_tear_down \
		},
#define CHEAT_DECLARE(body)

static struct cheat_procedure const cheat_procedures[] = {
#include __BASE_FILE__
};

static size_t const cheat_procedure_count = CHEAT_ARRAY_SIZE(cheat_procedures);

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
		static void cheat_test_##name(struct cheat_suite* cheat_suite) body
#define CHEAT_SET_UP(body) \
		static void cheat_set_up(void) body
#define CHEAT_TEAR_DOWN(body) \
		static void cheat_tear_down(void) body
#define CHEAT_DECLARE(body) \
		body

#define cheat_assert(expression) \
		cheat_check(cheat_suite, expression, #expression, __FILE__, __LINE__)

/*
The third pass continues past the end of this file, but
 the definitions for it end here.
*/

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
 returns NULL in case of a failure.
*/
__attribute__ ((__malloc__, __warn_unused_result__))
static void* cheat_malloc_total(size_t const size, size_t const extra_size) {
	if (extra_size > SIZE_MAX - size)
		return NULL;

	return malloc(size + extra_size);
}

/*
Safely reallocates memory for
 an array of size (count * size) and
 returns a pointer to the allocated region or
 returns NULL in case of a failure.
*/
__attribute__ ((__warn_unused_result__))
static void* cheat_realloc_array(void* const pointer,
		size_t const count, size_t const size) {
	if (count > SIZE_MAX / size)
		return NULL;

	return realloc(pointer, count * size);
}

/*
Prints a formatted string to a stream or
 fails safely in case the amount of conversion specifiers in
 the format string does not match the expected count.
*/
__attribute__ ((__format__ (printf, 1, 4), __io__, __nonnull__ (1)))
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
Prints a custom error message.
*/
__attribute__ ((__io__, __nonnull__))
static void cheat_print_error(char const* const message) {
	fputs(message, stderr); /* TODO Replace perror() with more detail. */
	fputs(": Failure\n", stderr);
}

/*
Clears a test suite.
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
	suite->program = "cheat";
	suite->harness = CHEAT_CALL;
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
	default:
		cheat_print_error("cheat_handle_outcome");
		exit(EXIT_FAILURE);
	}
}

/*
Prints a usage summary.
*/
__attribute__ ((__io__, __nonnull__))
static void cheat_print_usage(struct cheat_suite const* const suite) {
	fputs("Usage: ", suite->captured_stdout);
	fputs(suite->program, suite->captured_stdout);
	fputs(" --colors --help --unsafe\n", suite->captured_stdout);
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
		success = CHEAT_BACKGROUND_GREEN "." CHEAT_RESET;
		failure = CHEAT_BACKGROUND_RED ":" CHEAT_RESET;
		ignored = CHEAT_BACKGROUND_YELLOW "?" CHEAT_RESET;
		crashed = CHEAT_BACKGROUND_RED "!" CHEAT_RESET;
		break;
	case CHEAT_MINIMAL:
		print_bar = false;
		break;
	default:
		cheat_print_error("cheat_print_outcome");
		exit(EXIT_FAILURE);
	}

	if (print_bar) {
		switch (suite->outcome) {
		case CHEAT_SUCCESS:
			fputs(success, suite->captured_stdout);
			break;
		case CHEAT_FAILURE:
			fputs(failure, suite->captured_stdout);
			break;
		case CHEAT_IGNORED:
			fputs(ignored, suite->captured_stdout);
			break;
		case CHEAT_CRASHED:
			fputs(crashed, suite->captured_stdout);
			break;
		default:
			cheat_print_error("cheat_print_outcome");
			exit(EXIT_FAILURE);
		}
		fflush(suite->captured_stdout);
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
		successful_format = "%zu successful";
		and_string = " and ";
		failed_format = "%zu failed";
		of_string = " of ";
		run_format = "%zu run";
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
		separator_string = CHEAT_FOREGROUND_GRAY "---" CHEAT_RESET;
		successful_format = CHEAT_FOREGROUND_GREEN "%zu successful" CHEAT_RESET;
		and_string = " and ";
		failed_format = CHEAT_FOREGROUND_RED "%zu failed" CHEAT_RESET;
		of_string = " of ";
		run_format = CHEAT_FOREGROUND_YELLOW "%zu run" CHEAT_RESET;
		if (!any_failures)
			conclusion_string = CHEAT_FOREGROUND_GREEN "SUCCESS" CHEAT_RESET;
		else
			conclusion_string = CHEAT_FOREGROUND_RED "FAILURE" CHEAT_RESET;
		break;
	case CHEAT_MINIMAL:
		print_messages = false;
		print_summary = true;
		print_conclusion = false;
		print_zero = true;
		successful_format = "%zu";
		and_string = " ";
		failed_format = "%zu";
		of_string = " ";
		run_format = "%zu";
		break;
	default:
		cheat_print_error("cheat_print_summary");
		exit(EXIT_FAILURE);
	}

	if (print_messages && any_run) {
		fputc('\n', suite->captured_stdout);
		if (suite->messages != NULL) {
			size_t index;

			fputs(separator_string, suite->captured_stdout);
			fputc('\n', suite->captured_stdout);
			for (index = 0;
					index < suite->message_count;
					++index)
				fputs(suite->messages[index], suite->captured_stdout);
		}
		fputs(separator_string, suite->captured_stdout);
		fputc('\n', suite->captured_stdout);
	}
	if (print_summary) {
		if (print_zero || any_successes)
			cheat_print(successful_format, suite->captured_stdout,
					1, suite->tests_successful);
		if (print_zero || (any_successes && any_failures))
			fputs(and_string, suite->captured_stdout);
		if (print_zero || any_failures)
			cheat_print(failed_format, suite->captured_stdout,
					1, suite->tests_failed);
		if (print_zero || (any_successes || any_failures))
			fputs(of_string, suite->captured_stdout);
		cheat_print(run_format, suite->captured_stdout,
				1, suite->tests_run);
		fputc('\n', suite->captured_stdout);
	}
	if (print_conclusion) {
		fputs(conclusion_string, suite->captured_stdout);
		fputc('\n', suite->captured_stdout);
	}
}

__attribute__ ((__pure__, __warn_unused_result__))
static struct cheat_procedure const* cheat_find_test(struct cheat_suite* const suite,
		char const* const name) {
	struct cheat_procedure const* procedure;
	size_t index;

	for (index = 0;
			index < cheat_procedure_count;
			++index) {
		procedure = &cheat_procedures[index];
		if (procedure->type == CHEAT_TESTER
				&& strcmp(procedure->name, name) == 0)
			return procedure;
	}
	return NULL;
}

/* --- Things below this line are bad. --- */

/*
Adds a message in
 the form of a byte buffer to
 the queue of a test suite.
*/
static void cheat_append_message(struct cheat_suite* const suite,
		unsigned char const* const data, size_t const size) {
	size_t message_capacity;
	size_t message_count;
	void* messages;
	char* message;

	if (size == 0)
		return;

	if (suite->message_count == SIZE_MAX) {
		cheat_print_error("cheat_append_message");
		exit(EXIT_FAILURE);
	}
	message_count = suite->message_count + 1;

	message = cheat_malloc_total(size, 1); /* Memory B. */
	if (message == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memcpy(message, data, size);
	message[size] = '\0';

	if (suite->message_count == suite->message_capacity) {
		message_capacity = cheat_expand(suite->message_capacity);
		if (message_capacity == suite->message_capacity) {
			cheat_print_error("cheat_append_message");
			exit(EXIT_FAILURE);
		}
		messages = cheat_realloc_array(suite->messages,
				message_capacity,
				sizeof *suite->messages); /* Memory A. */
		if (messages == NULL) {
			perror("realloc");
			exit(EXIT_FAILURE);
		}
		suite->message_capacity = message_capacity;
		suite->messages = messages;
	}
	suite->messages[suite->message_count] = message;

	suite->message_count = message_count;
}

/*
Checks a single assertion and
 prints an error message if it fails.
*/
static void cheat_check(struct cheat_suite* const suite,
		int const result,
		char const* const assertion,
		char const* const filename,
		int const line) {
	bool print_assertion;
	char const* format;

	switch (suite->style) {
	case CHEAT_COLORFUL: /* TODO Inherit settings. */
		print_assertion = true;
		format = CHEAT_BOLD "%s:%d:"
				CHEAT_RESET " assertion failed: '"
				CHEAT_BOLD "%s"
				CHEAT_RESET "'\n";
		break;
	case CHEAT_PLAIN:
		print_assertion = true;
		format = "%s:%d: assertion failed: '%s'\n";
		break;
	case CHEAT_MINIMAL:
		print_assertion = false;
		break;
	default:
		cheat_print_error("cheat_check");
		exit(EXIT_FAILURE);
	}

	if (result != 0)
		return;

	if (print_assertion) {
		suite->outcome = CHEAT_FAILURE;
		if (suite->harness == CHEAT_FORK) {
			cheat_print(format, suite->captured_stdout,
					3, filename, line, assertion);
		} else {
			char* buffer = NULL;
			size_t len = BUFSIZ;
			size_t bufsize;

			do {
				int what;
				bufsize = len + 1;
				buffer = realloc(buffer, bufsize);
				what = snprintf(buffer, bufsize, format, /* TODO Only in C99. */
						filename, line, assertion);
				if (what == -1)
					exit(EXIT_FAILURE);
				len = (size_t )what;
			} while (bufsize != len + 1);

			cheat_append_message(suite, (unsigned char* )buffer, bufsize);
		}
	}
}

/*
Creates a subprocess and
 runs a test in it.
*/
static void cheat_run_isolated_test(struct cheat_procedure const* const test,
		struct cheat_suite* const suite) {
#if _POSIX_C_SOURCE >= 200112L
	pid_t pid;
	int pipefd[2];

	if (pipe(pipefd) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	} else if (pid == 0) { /* This is the subprocess. */
		if (close(pipefd[0]) == -1) {
			perror("close");
			exit(EXIT_FAILURE);
		}
		if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		/* Why exec? */
		if (execl(suite->program, suite->program, test->name, NULL) == -1) {
			perror("execl");
			exit(EXIT_FAILURE);
		}
		cheat_print_error("cheat_run_isolated_test");
		exit(EXIT_FAILURE);
	} else {
		ssize_t size;
		unsigned char buf[BUFSIZ];
		int status;

		if (close(pipefd[1]) == -1) {
			perror("close");
			exit(EXIT_FAILURE);
		}
		while ((size = read(pipefd[0], buf, sizeof buf)) != 0) {
			if (size == -1) {
				perror("read");
				exit(EXIT_FAILURE);
			}
			cheat_append_message(suite, buf, (size_t )size);
			/* This should contain the outcome, not the low bits of status. */
		}

		if (waitpid(pid, &status, 0) == -1) {
			perror("waitpid");
			exit(EXIT_FAILURE);
		}
		if (close(pipefd[0]) == -1) {
			perror("close");
			exit(EXIT_FAILURE);
		}
		/* TODO The outcome should be sent through the pipe instead. */
		if (WIFEXITED(status)) {
			suite->outcome = WEXITSTATUS(status);
			suite->status = WEXITSTATUS(status);
		} else
			suite->outcome = CHEAT_CRASHED;
	}
#elif defined _WIN32
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof (SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	HANDLE stdoutPipe_read;
	HANDLE stdoutPipe_write;
	CreatePipe(&stdoutPipe_read, &stdoutPipe_write, &sa, 0);

	STARTUPINFO si = {
		.cb = sizeof (STARTUPINFO),
		.dwFlags = STARTF_USESTDHANDLES,
		.hStdOutput = stdoutPipe_write
	};

	PROCESS_INFORMATION pi = {0};

	CHAR command[255];
	snprintf(command, 255, "%s %s", suite->program, test->name); /* TODO Only in C99. */

	CreateProcess(
		NULL,
		command,
		NULL,
		NULL,
		TRUE,
		0,
		NULL,
		NULL,
		&si,
		&pi);

	CloseHandle(stdoutPipe_write);

	DWORD len;
	DWORD maxlen = 255;
	CHAR buffer[255];

	do {
		ReadFile(stdoutPipe_read, buffer, maxlen, &len, NULL);
		buffer[len] = '\0'; /* TODO This is probably wrong. */
		cheat_append_message(suite, buffer, len);
	} while (len > 0);

	WaitForSingleObject(pi.hProcess, INFINITE);

	DWORD status;
	GetExitCodeProcess(pi.hProcess, &status);

	suite->status = (status & 0x80000000) ? CHEAT_CRASHED : status;

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
#else /* TODO Move into main and fall back to CHEAT_CALL. */
#warning "Isolated tests are unsupported. See the README file for help."
	cheat_print_error("cheat_run_isolated_test");
	exit(EXIT_FAILURE);
#endif
}

/*
Runs a test from
 a test suite.
*/
static enum cheat_outcome cheat_run_test(struct cheat_suite* const suite,
		struct cheat_procedure const* const procedure) {
	size_t index;

	suite->outcome = CHEAT_SUCCESS;

	for (index = 0;
			index < cheat_procedure_count;
			++index)
		if (cheat_procedures[index].type == CHEAT_UP_SETTER)
			((cheat_utility* )cheat_procedures[index].procedure)();

	((cheat_test* )procedure->procedure)(suite);

	for (index = 0;
			index < cheat_procedure_count;
			++index)
		if (cheat_procedures[index].type == CHEAT_DOWN_TEARER)
			((cheat_utility* )cheat_procedures[index].procedure)();

	return suite->outcome;
}

/*
Suite:
cheat
cheat --colors
cheat --minimal
cheat --unsafe
cheat --unsafe --colors
cheat --unsafe --minimal

Test:
cheat test
cheat --colors test
cheat --minimal test
cheat --unsafe test
cheat --unsafe --colors test
cheat --unsafe --minimal test

Help:
cheat --help
cheat --help --colors
*/
static void cheat_parse(struct cheat_suite* const suite,
		char* const* const tokens, size_t const count) {
	bool colors;
	bool help;
	bool minimal;
	bool test;
	bool safe;
	bool unsafe;
	size_t names;
	char const* name;
	bool parse_options;
	size_t index;

	colors = false;
	help = false;
	minimal = false;
	unsafe = false;
	names = 0;
	parse_options = true;
	for (index = 0;
			index < count;
			++index) {
		if (parse_options && tokens[index][0] == '-') {
			if (strcmp(tokens[index], "--") == 0)
				parse_options = false;
			else if (strcmp(tokens[index], "-c") == 0
					|| strcmp(tokens[index], "--colors") == 0)
				colors = true;
			else if (strcmp(tokens[index], "-h") == 0
					|| strcmp(tokens[index], "--help") == 0)
				help = true;
			else if (strcmp(tokens[index], "-m") == 0
					|| strcmp(tokens[index], "--minimal") == 0)
				minimal = true;
			else if (strcmp(tokens[index], "-s") == 0
					|| strcmp(tokens[index], "--safe") == 0)
				safe = true;
			else if (strcmp(tokens[index], "-u") == 0
					|| strcmp(tokens[index], "--unsafe") == 0)
				unsafe = true;
			else {
				cheat_print_error("cheat_parse");
				exit(EXIT_FAILURE);
			}
		} else {
			++names;
			name = tokens[index];
		}
	}
	test = names != 0;

	if (help && !minimal && !safe && !test && !unsafe) {
		if (colors)
			suite->style = CHEAT_COLORFUL;
		cheat_print_usage(suite);
	} else if (!(colors && minimal) && !help) {
		if (colors)
			suite->style = CHEAT_COLORFUL;
		if (minimal)
			suite->style = CHEAT_MINIMAL;
		if (safe)
			suite->harness = CHEAT_FORK;
		if (unsafe)
			suite->harness = CHEAT_CALL;
		if (test) {
			struct cheat_procedure const* procedure;

			procedure = cheat_find_test(suite, name);
			if (procedure == NULL)
				exit(EXIT_FAILURE);
			exit(cheat_run_test(suite, procedure)); /* TODO Something. */
		} else {
			for (index = 0;
					index < cheat_procedure_count;
					++index) {
				struct cheat_procedure const* procedure;

				procedure = &cheat_procedures[index];
				if (procedure->type != CHEAT_TESTER)
					continue;
				if (suite->harness == CHEAT_FORK)
					cheat_run_isolated_test(procedure, suite);
				else
					cheat_run_test(suite, procedure);
				cheat_handle_outcome(suite);
				cheat_print_outcome(suite);
			}
			cheat_print_summary(suite);
		}
	} else {
		cheat_print_error("cheat_parse");
		exit(EXIT_FAILURE);
	}
}

/*
Runs tests from the main test suite and
 returns EXIT_SUCCESS in case all tests passed or
 EXIT_FAILURE in case of an error.
*/
int main(int const count, char** const arguments) {
	struct cheat_suite suite;
	size_t result;

	cheat_initialize(&suite);
	suite.program = arguments[0];
	suite.harness = CHEAT_FORK;
	suite.style = CHEAT_PLAIN;
	cheat_parse(&suite, &arguments[1], (size_t )(count - 1));
	result = suite.tests_failed;
	cheat_clear(&suite);
	if (result == 0)
		return EXIT_SUCCESS;
	return EXIT_FAILURE;
}

#endif
