# CHEAT

CHEAT is a minimal unit testing framework for the C programming language.
Its primary use cases are small projects and systems for embedded platforms.
It has no dependencies and requires no installation or configuration.
The design philosophy is that time spent not writing tests is time wasted.
Only a header file and a statement block is needed.

	#include <cheat.h>

	CHEAT_TEST(mathematics_still_work,
		cheat_assert(2 + 2 == 4);
		cheat_assert(2 + 2 != 5);
	)

## Explanation

The working principle is best explained by a thought experiment.

> Imagine a source file including a header file.
> Then imagine the header file including the source file that included it.
> Now imagine doing that three times in a row within the same header file.
> Proceed to imagine redefining all of the identifiers each time.
> Finally imagine doing all of that with preprocessor directives.
> What you ended up with is CHEAT.

## History

The project was started on 2012-08-07 and
 is currently in passive development.
It was originally written by Guillermo "Tordek" Freschi for
 the entertainment and education of everyone in
 the ISO/IEC 9899 community on Freenode.
It was later picked up by Sampsa "Tuplanolla" Kiiskinen who
 grew tired of unit testing frameworks that suck and
 wondered what happened to the one that did not.

## License

CHEAT is free software and as such
 licensed under the simplified BSD license with two clauses.
The full license can be found in the `LICENSE` file that
 resides in the same directory as this file.
In short, copies and derivative works are permitted
 as long as they use the same license.

## Usage

### Installation

The core components are defined in a single header file, so
 the only necessary step is to download it.

	[user@computer ~]$ cd /usr/include
	[user@computer include]$ sudo wget -q https://github.com/Tuplanolla/cheat/raw/stable/cheat.h

### Writing Tests

Tests should be put in their own source file,

	[user@computer project]$ cat > tests.c

 which has to define `__BASE_FILE__`,

	#ifndef __BASE_FILE__
	#define __BASE_FILE__ __FILE__
	#endif

 if the compiler does not, and
 include `cheat.h`.

	#include <cheat.h>

For example GCC defines `__BASE_FILE__`, but
 the condition should be used anyway for portability.

Tests can then be defined with `CHEAT_TEST(name, block)`,
 global variables with `CHEAT_DECLARE(declarations)`,
 initialization with `CHEAT_SET_UP(block)` and
 finalization with `CHEAT_TEAR_DOWN(block)`.
It is also possible to
 ignore the outcome of a test with `CHEAT_IGNORE(block)` or
 skip running it altogether with `CHEAT_SKIP(block)`.
Additionally
 a reference to test can be retrieved with `CHEAT_GET(name)` or
 it can be directly called with `CHEAT_CALL(name)`.

Some examples are in the `cheat-example.c` file.

### Running Tests

Tests compile into an executable

	[user@computer project]$ gcc -o tests tests.c

 that takes care of running the tests and reporting their outcomes.

	[user@computer project]$ ./tests
	..:..??.!.
	---
	tests.c:88: assertion in 'philosophy_never_worked' failed: 'heap == stack'
	tests.c:103: assertion in 'important' failed: 'THIS == IMPORTANT_TEST'
	---
	6 successful and 2 failed of 10 run
	FAILURE

The executable runs tests in
 an isolated subprocess if possible, so
 the suite does not crash if
 one of the tests does.
The `-u` for `--unsafe` option allows
 running everything in the same process if
 `fork()` or `CreateProcess()` is unsupported, fails or
 something equally fun happens.
Conversely the `-s` for `--safe` option disallows
 running tests in the same process.
Additionally the `-d` for `--dangerous` option provides some stability by
 attempting to recover from signals like `SIGFPE` and `SIGSEGV`,
 most likely leading to undefined behavior.
Luckily undefined behavior is often defined enough behavior.

The results are reported in a format similar to
 what many popular C compilers produce.
As an added bonus the `-c` for `--colorful` option makes everything colorful,
 the `-m` for `--minimal` option makes the report machine readable and
 the `-x` for `--xml` option does nothing for good measure.

	[user@computer project]$ ./tests -m
	6 2 10

The default options depend on the target platform.

Option parsing can be disabled with `--` if
 some of the arguments begin with a dash when they should not.

### Individual Tests

More exotic use cases may require running individual tests.
The names of the tests follow the ones given in the source file, but
 there is also `-l` for `--list`.

	[user@computer project]$ ./tests -l
	Tests: mathematics_still_work
	       physics_still_work
	       philosophy_never_worked

They can be given as arguments to specify a single test to run.

	[user@computer project]$ ./tests mathematics_still_work
	[user@computer project]$ echo returned $?
	returned 0

### Complicated Tests

More complicated tests are easy to define with
 the assistance of CHEAT helpers that
 exist in `cheat-helpers.h`.

	#include <cheat-helpers.h>

Helpers cover things like
 stream redirection and
 signal handlers.

## Compatibility

CHEAT is designed for C, but
 also works with C++ after wading through a million warnings.

	[user@computer cheat]$ make -e CC=g++ -f makefile.gcc

Many features are targeted for POSIX and Linux systems, but
 the most critical ones have Windows compatibility as well.

There are a few `makefile`s for different compilers that
 show how to hammer out most problems.

	[user@computer cheat]$ make -f makefile.gcc

	computer# make -f makefile.tcc

	C:\CHEAT> makefile.bat

## Bugs and Limitations

CHEAT is naturally fickle, because
 it is built with C and
 heavy preprocessor abuse.
Some problems that are impossible to fix are
 collected into the following sections.

### Identifiers

Identifiers starting with
 `CHEAT_` and `cheat_` are
 reserved for internal use as
 C does not have namespaces.

### Commas

Using commas directly inside preprocessor directives like
 `CHEAT_TEST()` without `__VA_ARGS__` support causes
 everything that comes after them to
 be interpreted as extra arguments unless
 the commas are wrapped in parentheses,
 replaced with `CHEAT_COMMA` or
 in the worst case passed through
 the matching `CHEAT_COMMAS_` n `(x1, x2,` ... `)` where
 n is the amount of commas.

### Expressions

The expressions given to `cheat_assert()` should be
 at most 509 characters long since
 they are converted into string literals during compilation.

### Debugging

It is not possible to attach a breakpoint to `cheat_assert()`, because
 it is erased by the preprocessor, but
 using `cheat_check()` instead should work.

### Include Path

If `cheat.h` is placed in a global include directory (like `/usr/include`) and
 `__BASE_FILE__` is a relative path, then
 CHEAT will not work unless
 `cheat.h` is copied into the project directory.

### Base File

If `__BASE_FILE__` is defined in a file
 that does not directly include `cheat.h`, then
 CHEAT will refuse to cooperate until
 the definition is moved.

### Broken Compiler

If the compiler works like Microsoft C/C++ (commonly known as `cl.exe`) and
 defines either `__BASE_FILE__` or `__FILE__` wrong, then
 the test suite will be empty.
