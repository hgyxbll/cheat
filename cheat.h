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
 cheat_test_ are
 reserved for external use.
*/

#ifndef __BASE_FILE__

#error "The __BASE_FILE__ symbol is not defined. See the README file for help."

#endif

/*
Headers used here are also
 available externally even though
 they should not be.
*/

#include <limits.h> /* INT_MAX */
#include <stddef.h> /* NULL, size_t */
#include <stdint.h> /* SIZE_MAX */
#include <stdio.h> /* BUFSIZ, FILE, stderr, stdout */
#include <stdlib.h> /* EXIT_FAILURE */
#include <string.h>

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
	CHEAT_IGNORE, /* Anything could have happened. */
	CHEAT_SIGNAL_SEGV /* A segmentation fault happened. */
};

enum cheat_harness {
	CHEAT_CALL, /* Tests are called without any security measures. */
	CHEAT_SIGNAL_HANDLER, /* Tests are called and
			handling the signals they raise is attempted. */
	CHEAT_FORK /* Tests are called in their own subprocesses that
			are monitored. */
};

struct cheat_suite {
	size_t test_count; /* The amount of tests run so far. */
	size_t test_successes; /* The amount of successful tests so far. */
	size_t test_failures; /* The amount of failed tests so far. */

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

/**
Computes the compiled size of an array (not a pointer) and returns it.
**/
#define CHEAT_ARRAY_SIZE(array) \
		(sizeof array / sizeof *array)

/*
The first pass starts here.
*/

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
			cheat_suite->outcome = CHEAT_IGNORE; \
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

/**
Converts a size into an integer and
 returns it or
 the value -1 in case of an overflow.
**/
static int cheat_narrow(size_t const x) {
	return x > INT_MAX ? -1 : (int )x;
}

/**
Safely allocates memory for
 a block of size (size + extra_size) and
 returns a pointer to the allocated region or
 returns NULL in case of a failure.
**/
static void* cheat_malloc_total(size_t const size, size_t const extra_size) {
	if (extra_size > SIZE_MAX - size)
		return NULL;
	return malloc(size + extra_size);
}

/**
Safely reallocates memory for
 an array of size (count * size) and
 returns a pointer to the allocated region or
 returns NULL in case of a failure.
**/
static void* cheat_realloc_array(void* const pointer,
		size_t const count, size_t const size) {
	if (count > SIZE_MAX / size)
		return NULL;
	return realloc(pointer, count * size);
}

/**
Calculates the ratio (magnitude * numerator / denominator) and returns it.
**/
static size_t cheat_ratio(size_t const numerator, size_t const denominator,
		size_t const magnitude) { /* TODO Fix overflows. */
	return (numerator * magnitude + denominator / 2) / denominator;
}

/**
Prints a usage summary and
 returns the value 0 or
 the value -1 in case of an error.
**/
static int cheat_print_usage(struct cheat_suite const* const suite) {
	return printf("Usage: %s --no-fork\n", suite->program) < 0;
}

/**
Initializes a test suite.
**/
static void cheat_initialize(struct cheat_suite* const suite,
		char const* const program,
		enum cheat_harness const harness) {
	suite->test_count = 0;
	suite->test_successes = 0;
	suite->test_failures = 0;

	suite->outcome = CHEAT_INDETERMINATE;

	suite->message_count = 0;
	suite->message_capacity = 0;
	suite->messages = NULL;

	suite->program = program;

	suite->harness = harness;

	suite->captured_stdout = stdout;
}

/**
Prints the outcome of a single test.
**/
static void cheat_print_outcome(struct cheat_suite* const suite) {
	switch (suite->outcome) {
		case CHEAT_SUCCESS:
			fputc('.', suite->captured_stdout);
			++suite->test_successes;
			break;
		case CHEAT_FAILURE:
			fputc(':', suite->captured_stdout);
			++suite->test_failures;
			break;
		case CHEAT_IGNORE:
			fputc('?', suite->captured_stdout);
			break;
		case CHEAT_SIGNAL_SEGV:
			fputc('!', suite->captured_stdout);
			++suite->test_failures;
			break;
		default:
			exit(EXIT_FAILURE);
	} /* TODO Print summary. */
	++suite->test_count;
}

/**
Prints a summary of all tests.
**/
static void cheat_print_summary(struct cheat_suite* const suite) {
	fputs("]\n", suite->captured_stdout);
	if (suite->messages != NULL) {
		size_t index;

		for (index = 0;
				index < suite->message_count;
				++index) {
			fputs(suite->messages[index], suite->captured_stdout);
			free(suite->messages[index]);
		}

		free(suite->messages);
	}

	fprintf(suite->captured_stdout,
			"Total: %zu\nSuccessful: %zu\nFailed: %zu\n",
			suite->test_count, suite->test_successes, suite->test_failures);
	if (suite->test_count != 0)
		fprintf(suite->captured_stdout,
				"Ratio: %zu %%\n",
				cheat_ratio(suite->test_successes, suite->test_count, 100));
}

/**
Adds a message in
 the form of a byte buffer to
 the queue of a test suite.
**/
static void cheat_append_message(struct cheat_suite* const suite,
		unsigned char const* const data, size_t const size) {
	size_t message_count;
	void* messages;
	char* message;

	if (size == 0)
		return;

	if (suite->message_count == SIZE_MAX)
		exit(EXIT_FAILURE);
	message_count = suite->message_count + 1;

	message = cheat_malloc_total(size, 1);
	if (message == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memcpy(message, data, size);
	message[size] = '\0';

	/* TODO This is excessive. */
	messages = cheat_realloc_array(suite->messages,
			message_count, sizeof *suite->messages);
	if (messages == NULL) {
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	suite->messages = messages;
	suite->messages[suite->message_count] = message;

	suite->message_count = message_count;
}

/**
Checks a single assertion and
 prints an error message if it fails.
**/
static void cheat_check(struct cheat_suite* const suite,
		int const result,
		char const* const assertion,
		char const* const filename,
		int const line) {
	if (result != 0)
		return;

	suite->outcome = CHEAT_FAILURE;
	if (suite->harness == CHEAT_FORK) {
		fprintf(suite->captured_stdout,
				"%s:%d: Assertion failed: '%s'.\n",
				filename,
				line,
				assertion);
	} else {
		char* buffer = NULL;
		size_t len = BUFSIZ;
		size_t bufsize;

		do {
			int what;
			bufsize = len + 1;
			buffer = realloc(buffer, bufsize);
			what = snprintf(buffer, bufsize, /* TODO Only in C99. */
					"%s:%d: Assertion failed: '%s'.\n",
					filename,
					line,
					assertion);
			if (what == -1)
				exit(EXIT_FAILURE);
			len = (size_t )what;
		} while (bufsize != len + 1);

		cheat_append_message(suite, (unsigned char *)buffer, bufsize);
	}
}

/**
Creates a subprocess and
 runs a test in it.
**/
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
		if (execl(suite->program, suite->program, test->name, NULL) == -1)
			perror("execl");
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
			suite->outcome = CHEAT_SIGNAL_SEGV;
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

	suite->status = (status & 0x80000000) ? CHEAT_SIGNAL_SEGV : status;

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

#else /* TODO Move. */

#warning "Running isolated tests is not supported in this environment. You will have to use --no-fork."
	fputs("Running isolated tests is not supported in this environment. You will have to use --no-fork.\n", stderr);
	exit(EXIT_FAILURE);

#endif

}

/**
Runs a test.
**/
static int cheat_run_test(struct cheat_procedure const* const test,
		struct cheat_suite* const suite) {
	size_t index;

	suite->outcome = CHEAT_SUCCESS;

	for (index = 0;
			index < cheat_procedure_count;
			++index)
		if (cheat_procedures[index].type == CHEAT_UP_SETTER)
			((cheat_utility* )cheat_procedures[index].procedure)();

	((cheat_test* )test->procedure)(suite);

	for (index = 0;
			index < cheat_procedure_count;
			++index)
		if (cheat_procedures[index].type == CHEAT_DOWN_TEARER)
			((cheat_utility* )cheat_procedures[index].procedure)();

	return suite->outcome;
}

/**
Parses options,
 runs test cases with them and
 returns the total amount of failures or
 the value -1 in case of an error.
**/
int main(int const count, char** const arguments) {
	struct cheat_suite suite;
	size_t index;

	cheat_initialize(&suite, arguments[0], CHEAT_FORK);

	if (count > 1) { /* TODO Use a proper parser. */
		if (arguments[1][0] == '-') {
			if (strcmp(arguments[1], "-n") == 0
					|| strcmp(arguments[1], "--no-fork") == 0)
				suite.harness = CHEAT_CALL;
		} else { /* A leaky abstraction? */
			for (index = 0;
					index < cheat_procedure_count;
					++index) {
				struct cheat_procedure const* procedure = &cheat_procedures[index];

				if (procedure->type == CHEAT_TESTER
						&& strcmp(arguments[1], procedure->name) == 0)
					return cheat_run_test(procedure, &suite);
					/* TODO Return a value that is suitable for WEXITSTATUS. */
			}

			return -1;
		}

		cheat_print_usage(&suite); /* TODO Add --help somewhere. */
	}

	fputs("[", suite.captured_stdout); /* TODO Move. */

	for (index = 0;
			index < cheat_procedure_count;
			++index) {
		struct cheat_procedure const* procedure = &cheat_procedures[index];

		if (procedure->type != CHEAT_TESTER)
			continue;

		if (suite.harness == CHEAT_FORK)
			cheat_run_isolated_test(procedure, &suite);
		else
			cheat_run_test(procedure, &suite);

		cheat_print_outcome(&suite);
	}

	cheat_print_summary(&suite);

	return cheat_narrow(suite.test_failures);
}

#endif
