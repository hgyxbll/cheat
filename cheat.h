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
 reserved for internal use.
*/

#ifndef __BASE_FILE__
#error "The __BASE_FILE__ symbol is not defined. See the README file for help."
#endif

#include <stddef.h> /* NULL, size_t */
#include <stdio.h> /* FILE, fprintf(), fputc(), fputs(), perror(), snprintf(),
		stderr, stdout */

enum cheat_status {
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

	enum cheat_status status; /* The outcome of
			the most recently completed test. */

	size_t message_count; /* The amount of messages so far. */
	size_t message_capacity; /* The amount of messages until
			more memory has to be allocated. */
	char** messages; /* The messages so far. */

	char* program; /* The program name given to main(). */

	enum cheat_harness harness; /* The security measures to use. */

	FILE* captured_stdout; /* TODO Document. */
};

typedef void cheat_test(struct cheat_suite*);

struct cheat_test {
	char const* name; /* The name to use for
			generating identifiers and
			reporting test results. */
	cheat_test* test; /* The procedure to call. */
};

#define CHEAT_PASS 1

/*
The system headers
 stddef.h and
 stdio.h are
 available to the user even though
 they should not be.
*/

#define TEST_IGNORE(name, body) TEST(name, { \
	cheat_suite->status = CHEAT_IGNORE; \
}) /* TODO What? */

#define TEST(name, body) \
		static void test_##name(struct cheat_suite*);
#define SET_UP(body) \
		static void cheat_set_up(void);
#define TEAR_DOWN(body) \
		static void cheat_tear_down(void);
#define GLOBALS(body)

#include __BASE_FILE__

#undef TEST
#undef SET_UP
#undef TEAR_DOWN
#undef GLOBALS

#undef CHEAT_PASS

#include <limits.h> /* INT_MAX */
#include <stdlib.h> /* EXIT_FAILURE, exit(), free(), malloc(), realloc() */
#include <string.h> /* memcpy() */

static void cheat_suite_init(struct cheat_suite* suite, char* program) {
	suite->test_count = 0;
	suite->test_successes = 0;
	suite->test_failures = 0;

	suite->status = CHEAT_INDETERMINATE;

	suite->message_count = 0;
	suite->message_capacity = 0;
	suite->messages = NULL;

	suite->program = program;
	suite->harness = CHEAT_CALL;
	suite->captured_stdout = stdout;
}

static void cheat_suite_summary(struct cheat_suite* suite) {
	if (suite->messages != NULL) {
		size_t index;

		fputs("\n", suite->captured_stdout);
		for (index = 0;
				index < suite->message_count;
				++index) {
			fputs(suite->messages[index], suite->captured_stdout);
			free(suite->messages[index]);
		}

		free(suite->messages);
	}

	fprintf(suite->captured_stdout, "\n%zu failed tests of %zu tests run.\n",
			suite->test_failures, suite->test_count);
}

static void cheat_test_end(struct cheat_suite* suite) {
	++suite->test_count;

	switch (suite->status) {
		case CHEAT_SUCCESS:
			fputc('.', suite->captured_stdout);
			break;
		case CHEAT_FAILURE:
			fputc('F', suite->captured_stdout);
			++suite->test_failures;
			break;
		case CHEAT_IGNORE:
			fputc('I', suite->captured_stdout);
			break;
		case CHEAT_SIGNAL_SEGV:
			fputc('S', suite->captured_stdout);
			++suite->test_failures;
			break;
		default:
			exit(-1);
	}
}

static void cheat_log_append(struct cheat_suite* suite, char* message, size_t len) {
	char* buf;

	if (len == 0) {
		return;
	}

	buf = malloc(len + 1);
	memcpy(buf, message, len);

	buf[len] = '\0';

	++suite->message_count;
	suite->messages = realloc(suite->messages, (suite->message_count + 1) * sizeof (char*)); /* TODO What char*? */
	suite->messages[suite->message_count - 1] = buf; /* We give up our buffer! */
}

static void cheat_test_assert(struct cheat_suite* suite,
		int result,
		char const* assertion,
		char const* filename,
		int line) {
	if (result != 0)
		return;

	suite->status = CHEAT_FAILURE;
	if (suite->harness == CHEAT_FORK) {
		fprintf(suite->captured_stdout,
				"%s:%d: Assertion failed: '%s'.\n",
				filename,
				line,
				assertion);
	} else {
		char* buffer = NULL;
		size_t len = 255;
		size_t bufsize;

		do {
			int what;
			bufsize = (len + 1);
			buffer = realloc(buffer, bufsize);
			what = snprintf(buffer, bufsize,
					"%s:%d: Assertion failed: '%s'.\n",
					filename,
					line,
					assertion);
			if (what == -1)
				exit(42);
			len = (size_t )what;
		} while (bufsize != (len + 1));

		cheat_log_append(suite, buffer, bufsize);
	}
}

static int run_test(struct cheat_test const* test, struct cheat_suite* suite) {
	suite->status = CHEAT_SUCCESS;

	cheat_set_up();
	(test->test)(suite);
	cheat_tear_down();

	return suite->status;
}

#if _POSIX_C_SOURCE >= 200112L

#include <unistd.h> /* STDOUT_FILENO, close(), dup2(), execl(), fork(), pipe(),
		read() */
#include <sys/types.h> /* pid_t, ssize_t */
#include <sys/wait.h> /* WIFEXITED(), WEXITSTATUS(), waitpid() */

#elif defined _WIN32

#include <windows.h> /* CloseHandle(), CloseHandle(), CreatePipe(),
		CreateProcess(), GetExitCodeProcess(), ReadFile(),
		WaitForSingleObject() */

#endif

static void run_isolated_test(
		struct cheat_test const* test,
		struct cheat_suite* suite) {

#if _POSIX_C_SOURCE >= 200112L

	pid_t pid;
	int pipefd[2];

	pipe(pipefd);
	pid = fork();

	if (pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	} else if (pid == 0) {
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		execl(suite->program, suite->program, test->name, NULL);
		exit(EXIT_FAILURE);
	} else {
		int status;
		char buf[255];
		ssize_t len;

		close(pipefd[1]);
		while ((len = read(pipefd[0], buf, 255)) != 0) {
			if (len == -1)
				exit(42);
			buf[len] = 0;
			cheat_log_append(suite, buf, (size_t )len + 1);
		}

		waitpid(pid, &status, 0);
		close(pipefd[0]);
		suite->status = WIFEXITED(status) ? WEXITSTATUS(status)
		                                            : CHEAT_SIGNAL_SEGV;
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
	snprintf(command, 255, "%s %s", suite->program, test->name);

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
		buffer[len] = '\0';
		cheat_log_append(suite, buffer, len);
	} while (len > 0);

	WaitForSingleObject(pi.hProcess, INFINITE);

	DWORD status;
	GetExitCodeProcess(pi.hProcess, &status);

	suite->status = (status & 0x80000000) ? CHEAT_SIGNAL_SEGV
													: status;

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

#else /* TODO Move. */

#warning "Running isolated tests is not supported in this environment. You will have to use --no-fork."
	fputs("Running isolated tests is not supported in this environment. You will have to use --no-fork.\n", stderr);
	exit(EXIT_FAILURE);

#endif

}

#define CHEAT_PASS 2

#define TEST(name, body) { #name, test_##name },
#define SET_UP(body)
#define TEAR_DOWN(body)
#define GLOBALS(body)

static struct cheat_test const cheat_tests[] = {
#include __BASE_FILE__
};

static size_t const cheat_test_count = sizeof cheat_tests / sizeof *cheat_tests;

#undef TEST
#undef SET_UP
#undef TEAR_DOWN
#undef GLOBALS

#undef CHEAT_PASS

/**
Converts a size type into an integer and
 returns it or
 the value -1 in case of an overflow.
**/
static int cheat_narrow(size_t const x) {
	return x > INT_MAX ? -1 : (int )x;
}

/**
Prints a usage summary and
 returns the value 0 or
 the value -1 in case of an error.
**/
static int cheat_print_usage(struct cheat_suite const* suite) {
	return printf("Usage: %s --no-fork\n", suite->program) < 0;
}

/**
Parses options,
 runs test cases with them and
 returns the total amount of failures or
 the value -1 in case of an error.
**/
int main(int count, char** arguments) {
	struct cheat_suite suite;
	size_t index;

	cheat_suite_init(&suite, arguments[0]);

	suite.harness = CHEAT_FORK;

	if (count > 1) { /* TODO Use a proper parser. */
		if (arguments[1][0] == '-') {
			if (strcmp(arguments[1], "-n") == 0
					|| strcmp(arguments[1], "--no-fork") == 0)
				suite.harness = CHEAT_CALL;
		} else {
			for (index = 0;
					index < cheat_test_count;
					++index) {
				struct cheat_test const current_test = cheat_tests[index];

				if (strcmp(arguments[1], current_test.name) == 0) {
					return run_test(&current_test, &suite);
				}
			}

			cheat_print_usage(&suite); /* TODO Add --help. */

			return -1;
		}
	}

	for (index = 0;
			index < cheat_test_count;
			++index) {
		struct cheat_test const current_test = cheat_tests[index];

		if (suite.harness == CHEAT_FORK)
			run_isolated_test(&current_test, &suite);
		else
			run_test(&current_test, &suite);

		cheat_test_end(&suite);
	}

	cheat_suite_summary(&suite);

	return cheat_narrow(suite.test_failures);
}

#define CHEAT_PASS 3

/*
The symbols defined here are used in the first pass.
*/

#define TEST(name, body) \
		static void test_##name(struct cheat_suite* cheat_suite) body
#define SET_UP(body) \
		static void cheat_set_up(void) body
#define TEAR_DOWN(body) \
		static void cheat_tear_down(void) body
#define GLOBALS(body) \
		body

#define cheat_assert(boolean) \
		cheat_test_assert(cheat_suite, boolean, #boolean, __FILE__, __LINE__)

#undef CHEAT_PASS

#endif
