# CHEAT

CHEAT stands for C Header Embedded Automated Testing or something like that.
It is a minimal unit testing framework for the C programming language.
It has no dependencies and requires no installation or configuration.
Only a header file and a test case is needed.

	#include <cheat.h>

	CHEAT_TEST(mathematics_still_work,
		cheat_assert(2 + 2 == 4);
		cheat_assert_not(2 + 2 == 5);
	)

Section 1 presents the basic use case.
You can skip to section 2 if you are only looking for an overview.

## 1   Getting Started

In this introduction it is assumed that you are running a Linux system with
 the GNU Core Utilities and the GNU Compiler Collection installed.
None of that is necessary, but it makes the introduction easier to follow.
Compatibility with other tools and operating systems is addressed in section 4.

### 1.1   Preparing

First you need to download the main header

	[user@computer ~]$ wget https://github.com/Tuplanolla/cheat/raw/stable/cheat.h

 and move it to a suitable location like the global search path

	[user@computer ~]$ sudo mv -n cheat.h /usr/include

 or the working directory of your project.

	[user@computer ~]$ mv -n cheat.h project

Then you are ready to write tests.

	[user@computer ~]$ cd project

### 1.2   Writing Tests

Tests go into their own source file.

	[user@computer project]$ cat > tests.c
	#include <cheat.h>

It is an ordinary file with the exception that it is processed more than once.
Therefore you must wrap all top level declarations and definitions with
 the appropriate preprocessor directives.
The reason for that is explained in section 2.3.

Including the main header is enough to get an empty test suite, but
 such a thing is not very useful beyond making sure everything is set up right.
The next step is to define tests.
You can define tests with `CHEAT_TEST()` and
 their success conditions called assertions with `cheat_assert()`.
Doing so is demonstrated in the example file.

	[user@computer project]$ wget https://github.com/Tuplanolla/cheat/raw/stable/example.c
	[user@computer project]$ mv example.c tests.c

It is important to note that the utility procedures defined with
 `CHEAT_SET_UP()` and `CHEAT_TEAR_DOWN()` are run for every test and
 the state of mutable global variables before the former is undefined.

A detailed explanation of the entire interface is in section 7.1.

### 1.3   Running Tests

Tests compile into an executable

	[user@computer project]$ gcc -I . -o tests tests.c

 that takes care of running the tests and reporting their outcomes.
There are two things that need to be taken care of when compiling the file.
First you have to add the directory of the test suite to the search path, as
 is done here with `-I .`.
Then you have to make `__BASE_FILE__` point to the test suite, by
 using `-D __BASE_FILE__=\"tests.c\"`, if the compiler does not.
The reason is related to the previous oddity and is in section 2.3.

The resulting executable runs tests in a security harness if possible, so
 the suite does not crash if one of its tests does.

	[user@computer project]$ ./tests
	..:..??..!..
	---
	tests.c:87: assertion in 'philosophy_never_worked' failed: 'heap == stack'
	tests.c:110: assertion in 'important' failed: 'THIS_TEST == IMPORTANT_TEST'
	---
	8 successful and 2 failed of 12 run
	FAILURE

The results are reported in four parts.

The first part is a progress bar, where

* a success is a green dot,
* a failure is a red colon,
* a crash is a red exclamation mark,
* a time out is a yellow exclamation mark and
* an ignored outcome is signaled with a yellow question mark.

The second part contains diagnostic messages in
 a format similar to what many popular C compilers produce.

The third part, omitted here, would hold the captured output and error streams.

The fourth and last part, which is always shown, briefly summarizes all of that.

You can control the behavior of the test suite with command line options.
They are presented in sections 3.3 and 7.2.

## 2   Overview

### 2.1   License

CHEAT is free software and as such
 licensed under the simplified BSD license with two clauses.
The full license can be found in the `LICENSE` file that
 resides in the same directory as this file.
In short, copies and derivative works are permitted
 as long as they use the same license.

It would be licensed under the GNU GPL, but
 the authors felt that such a decision would hinder its adoption.

### 2.2   History

The project was started on 2012-08-07 and will be first released on 2014-08-07.
It was originally written by Guillermo "Tordek" Freschi for
 the entertainment and education of everyone in
 the ISO/IEC 9899 community on Freenode.
The prototype was later picked up by Sampsa "Tuplanolla" Kiiskinen who
 grew tired of unit testing frameworks that suck and
 wondered what happened to the one that did not.
It was rewritten, stuffed with new features and
 finally audited in a small scale.

### 2.3   Implementation

The working principle is best explained by a thought experiment.

> Imagine a source file including a header file.
> Then imagine the header file including the source file that included it.
> Now imagine doing that three times in a row within the same header file.
> Proceed to imagine redefining all of the identifiers each time.
> Finally imagine doing all of that with preprocessor directives.
> What you ended up with is CHEAT.

It sounds strange, but it works.

### 2.4   Contributing

The support for Windows and other more exotic operating systems is not complete.
For example stream capturing is currently very limited without POSIX interfaces.

Contributions in the forms of feedback and pull requests are all very welcome!

## 3   Advanced Usage

### 3.1   Other Files

There are other useful files as mentioned earlier.

	[user@computer ~]$ git clone git@github.com:Tuplanolla/cheat.git
	[user@computer ~]$ cd cheat

There is the example and makefiles for it,

	[user@computer cheat]$ make test clean

 tests for corner cases

	[user@computer cheat]$ ls tests
	[user@computer cheat]$ ./test

 and other fun things.

	[user@computer cheat]$ xdot streams.dot
	[user@computer cheat]$ man ./cheat.7
	[user@computer cheat]$ cat LICENSE

### 3.2   Additional Features

There are things like `CHEAT_SET_UP(declarations)` and `CHEAT_CALL(name)`.

See section 7 or wait for this to be completed.

### 3.3   Command Line Options

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

More exotic use cases may require running individual tests.
The names of the tests follow the ones given in the source file, but
 there is also `-l` for `--list`.

	[user@computer project]$ ./tests -l
	Tests: mathematics_still_work
	       physics_still_work
	       philosophy_never_worked

They can be given as arguments to specify the tests to run.

	[user@computer project]$ ./tests -m physics_still_work physics_still_work
	2 0 2
	[user@computer project]$ echo returned $?
	returned 0

The incomplete `-t` for `--timed` and
 `-e` for `--eternal` allow controlling isolated tests that get stuck while
 `-q` for `--quiet` and
 `-n` for `--noisy` determine whether standard streams are captured and printed.

As an added bonus the `-c` for `--colorful` option makes everything colorful,
 the `-m` for `--minimal` option makes the report machine readable and
 the `-x` for `--xml` option does nothing for good measure.

	[user@computer project]$ ./tests -m
	8 2 12

The default options depend on the target platform.

Option parsing can be disabled with `--` if
 some of the arguments begin with a dash when they should not.

Extra procedures like `cheat_assert_string(actual, expected)` and
 other convenient things are available in `cheats.h`.

### 3.4   Design Decisions

No tests and empty tests result in a success.

> Every predicate is true for the empty set, so why not choose favorably?

More about these kinds of things later.

## 4   Portability

### 4.1   Standards Compliance

The whole thing is very standards compliant.
It follows ANSI this, ISO that and IEEE what.
The details will be written later.

Many features are targeted for POSIX systems, but
 the most critical ones have Windows compatibility as well.

There are a few `makefile`s for different compilers that
 show how to hammer out most problems.

	[user@computer cheat]$ make -f makefile.gcc

	computer# make -f makefile.tcc

	E:\CHEAT> makefile.bat

You can see screenshots of these in section 7.

### 4.2   Language Compatibility

CHEAT is designed for C, but
 also works with C++ after wading through a million warnings.

	[user@computer cheat]$ make -e CC=g++ -f makefile.gcc

## 5   Bugs and Limitations

CHEAT is naturally fickle, because
 it is built with C and
 heavy preprocessor abuse.
Some problems that are impossible to fix are
 collected into the following sections.

### 5.1   Identifiers

Identifiers starting with
 `CHEAT_` and `cheat_` are
 reserved for internal use as
 C does not have namespaces.

### 5.2   File Definitions

The preprocessor directive `__BASE_FILE__` has to be defined like

	#ifndef __BASE_FILE__
	#define __BASE_FILE__ __FILE__
	#endif

 if the compiler fails to do so.
For example GCC defines `__BASE_FILE__`, but
 the condition should be used anyway for portability.

### 5.3   Broken Compiler

If the compiler works like Microsoft C/C++ (commonly known as `cl.exe`) and
 defines either `__BASE_FILE__` or `__FILE__` wrong, then
 the test suite will be empty as
 long as it is not manually fed to the compiler.

### 5.4   Include Path

If `cheat.h` is placed in a global include directory (like `/usr/include`) and
 `__BASE_FILE__` is a relative path, then
 CHEAT will not work unless
 `cheat.h` is copied into the project directory.

### 5.5   Commas

Using commas directly inside preprocessor directives like
 `CHEAT_TEST()` without `__VA_ARGS__` support causes
 everything that comes after them to
 be interpreted as extra arguments unless
 the commas are wrapped in parentheses,
 replaced with `CHEAT_COMMA` or
 in the worst case passed through
 the matching `CHEAT_COMMAS_` n `(x1, x2,` ... `)` where
 n is the amount of commas.

### 5.6   Expressions

The expressions given to `cheat_assert()` should be
 at most 509 characters long since
 they are converted into string literals during compilation.

### 5.7   Debugging

It is not possible to attach a breakpoint to `cheat_assert()`, because
 it is erased by the preprocessor, but
 using `cheat_check()` instead should work.

### 6   Screenshots

Everyone likes pretty pictures.

Here is CHEAT being compiled with the GNU Compiler Collection and
 run in the Xfce terminal emulator of a Linux distribution.

![Screenshot](https://raw.github.com/Tuplanolla/cheat/master/xfce.png)

Here is CHEAT being compiled with Microsoft C/C++ and
 run in the command prompt of Windows XP.

![Another Screenshot](https://raw.github.com/Tuplanolla/cheat/master/xp.png)

Here is CHEAT being compiled with Borland Turbo C and
 run in the default shell of FreeDOS.

![Yet Another Screenshot](https://raw.github.com/Tuplanolla/cheat/master/dos.png)

## 7   Reference

### 7.1   Compilation Things

These form the primary interface.

* `CHEAT_TEST(name, statements)`
* `CHEAT_DECLARE(declarations)`
* `CHEAT_SET_UP(statements)`
* `CHEAT_TEAR_DOWN(statements)`
* `CHEAT_IGNORE(statements)`
* `CHEAT_SKIP(statements)`

These are the most essential part of it.

* `cheat_assert(bool expected)`
* `cheat_assert_not(bool unexpected)`

These exist for convenience.

* `CHEAT_GET(name)`
* `CHEAT_CALL(name)`

This allows some configuration.

* `CHEAT_TIME`

These help work around issues.

* `CHEAT_COMMA`
* `CHEAT_COMMAS(...)`
* `CHEAT_COMMAS_1(x1, x2)`
* `int CHEAT_OFFSET`
* `CHEAT_UNWRAP(name)`
* `CHEAT_NO_WRAP`

This is for those who want to do more.

* `CHEAT_NO_MAIN`

These exist by accident.

* `CHEAT_BEGIN`
* `CHEAT_END`
* `size_t CHEAT_LIMIT`
* `type CHEAT_CAST(type, expression)`
* `size_t CHEAT_INTEGER_LENGTH(type)`
* `size_t CHEAT_FLOATING_LENGTH(type)`

### 7.2   Execution Things

These are available.

* `-c` for `--colorful`
* `-d` for `--dangerous`
* `-e` for `--eternal`
* `-h` for `--help`
* `-l` for `--list`
* `-m` for `--minimal`
* `-n` for `--noisy`
* `-p` for `--plain`
* `-s` for `--safe`
* `-t` for `--timed`
* `-u` for `--unsafe`
* `-v` for `--version`
* `-q` for `--quiet`

This one is not.

* `--__hidden`

### 7.3   Extension Things

These are available as extensions (using `cheats.h` in addition to `cheat.h`).

* `cheat_assert_char(char actual, char expected)`
* `cheat_assert_not_char(char actual, char unexpected)`
* `cheat_assert_short_int(short int actual, short int expected)`
* `cheat_assert_not_short_int(short int actual, short int unexpected)`
* `cheat_assert_short_unsigned_int(short unsigned int actual, short unsigned int expected)`
* `cheat_assert_not_short_unsigned_int(short unsigned int actual, short unsigned int unexpected)`
* `cheat_assert_int(int actual, int expected)`
* `cheat_assert_not_int(int actual, int unexpected)`
* `cheat_assert_unsigned_int(unsigned int actual, unsigned int expected)`
* `cheat_assert_not_unsigned_int(unsigned int actual, unsigned int unexpected)`
* `cheat_assert_long_int(long int actual, long int expected)`
* `cheat_assert_not_long_int(long int actual, long int unexpected)`
* `cheat_assert_long_unsigned_int(long unsigned int actual, long unsigned int expected)`
* `cheat_assert_not_long_unsigned_int(long unsigned int actual, long unsigned int unexpected)`
* `cheat_assert_double(double actual, double expected)`
* `cheat_assert_not_double(double actual, double unexpected)`
* `cheat_assert_size(size_t actual, size_t expected)`
* `cheat_assert_not_size(size_t actual, size_t unexpected)`
* `cheat_assert_ptrdiff(ptrdiff_t actual, ptrdiff_t expected)`
* `cheat_assert_not_ptrdiff(ptrdiff_t actual, ptrdiff_t unexpected)`
* `cheat_assert_long_long_int(long long int actual, long long int expected)`
* `cheat_assert_not_long_long_int(long long int actual, long long int unexpected)`
* `cheat_assert_long_long_unsigned_int(long long unsigned int actual, long long unsigned int expected)`
* `cheat_assert_not_long_long_unsigned_int(long long unsigned int actual, long long unsigned int unexpected)`
* `cheat_assert_float(float actual, float expected)`
* `cheat_assert_not_float(float actual, float unexpected)`
* `cheat_assert_long_double(long double actual, long double expected)`
* `cheat_assert_not_long_double(long double actual, long double unexpected)`
* `cheat_assert_int8(int8_t actual, int8_t expected)`
* `cheat_assert_not_int8(int8_t actual, int8_t unexpected)`
* `cheat_assert_uint8(uint8_t actual, uint8_t expected)`
* `cheat_assert_not_uint8(uint8_t actual, uint8_t unexpected)`
* `cheat_assert_int16(int16_t actual, int16_t expected)`
* `cheat_assert_not_int16(int16_t actual, int16_t unexpected)`
* `cheat_assert_uint16(uint16_t actual, uint16_t expected)`
* `cheat_assert_not_uint16(uint16_t actual, uint16_t unexpected)`
* `cheat_assert_int32(int32_t actual, int32_t expected)`
* `cheat_assert_not_int32(int32_t actual, int32_t unexpected)`
* `cheat_assert_uint32(uint32_t actual, uint32_t expected)`
* `cheat_assert_not_uint32(uint32_t actual, uint32_t unexpected)`
* `cheat_assert_int64(int64_t actual, int64_t expected)`
* `cheat_assert_not_int64(int64_t actual, int64_t unexpected)`
* `cheat_assert_uint64(uint64_t actual, uint64_t expected)`
* `cheat_assert_not_uint64(uint64_t actual, uint64_t unexpected)`
* `cheat_assert_int_fast8(int_fast8_t actual, int_fast8_t expected)`
* `cheat_assert_not_int_fast8(int_fast8_t actual, int_fast8_t unexpected)`
* `cheat_assert_uint_fast8(uint_fast8_t actual, uint_fast8_t expected)`
* `cheat_assert_not_uint_fast8(uint_fast8_t actual, uint_fast8_t unexpected)`
* `cheat_assert_int_fast16(int_fast16_t actual, int_fast16_t expected)`
* `cheat_assert_not_int_fast16(int_fast16_t actual, int_fast16_t unexpected)`
* `cheat_assert_uint_fast16(uint_fast16_t actual, uint_fast16_t expected)`
* `cheat_assert_not_uint_fast16(uint_fast16_t actual, uint_fast16_t unexpected)`
* `cheat_assert_int_fast32(int_fast32_t actual, int_fast32_t expected)`
* `cheat_assert_not_int_fast32(int_fast32_t actual, int_fast32_t unexpected)`
* `cheat_assert_uint_fast32(uint_fast32_t actual, uint_fast32_t expected)`
* `cheat_assert_not_uint_fast32(uint_fast32_t actual, uint_fast32_t unexpected)`
* `cheat_assert_int_fast64(int_fast64_t actual, int_fast64_t expected)`
* `cheat_assert_not_int_fast64(int_fast64_t actual, int_fast64_t unexpected)`
* `cheat_assert_uint_fast64(uint_fast64_t actual, uint_fast64_t expected)`
* `cheat_assert_not_uint_fast64(uint_fast64_t actual, uint_fast64_t unexpected)`
* `cheat_assert_int_least8(int_least8_t actual, int_least8_t expected)`
* `cheat_assert_not_int_least8(int_least8_t actual, int_least8_t unexpected)`
* `cheat_assert_uint_least8(uint_least8_t actual, uint_least8_t expected)`
* `cheat_assert_not_uint_least8(uint_least8_t actual, uint_least8_t unexpected)`
* `cheat_assert_int_least16(int_least16_t actual, int_least16_t expected)`
* `cheat_assert_not_int_least16(int_least16_t actual, int_least16_t unexpected)`
* `cheat_assert_uint_least16(uint_least16_t actual, uint_least16_t expected)`
* `cheat_assert_not_uint_least16(uint_least16_t actual, uint_least16_t unexpected)`
* `cheat_assert_int_least32(int_least32_t actual, int_least32_t expected)`
* `cheat_assert_not_int_least32(int_least32_t actual, int_least32_t unexpected)`
* `cheat_assert_uint_least32(uint_least32_t actual, uint_least32_t expected)`
* `cheat_assert_not_uint_least32(uint_least32_t actual, uint_least32_t unexpected)`
* `cheat_assert_int_least64(int_least64_t actual, int_least64_t expected)`
* `cheat_assert_not_int_least64(int_least64_t actual, int_least64_t unexpected)`
* `cheat_assert_uint_least64(uint_least64_t actual, uint_least64_t expected)`
* `cheat_assert_not_uint_least64(uint_least64_t actual, uint_least64_t unexpected)`
* `cheat_assert_intmax(intmax_t actual, intmax_t expected)`
* `cheat_assert_not_intmax(intmax_t actual, intmax_t unexpected)`
* `cheat_assert_uintmax(uintmax_t actual, uintmax_t expected)`
* `cheat_assert_not_uintmax(uintmax_t actual, uintmax_t unexpected)`
* `cheat_assert_intptr(intptr_t actual, intptr_t expected)`
* `cheat_assert_not_intptr(intptr_t actual, intptr_t unexpected)`
* `cheat_assert_uintptr(uintptr_t actual, uintptr_t expected)`
* `cheat_assert_not_uintptr(uintptr_t actual, uintptr_t unexpected)`
* `cheat_assert_float_complex(float actual, float expected)`
* `cheat_assert_not_float_complex(float actual, float unexpected)`
* `cheat_assert_double_complex(double actual, double expected)`
* `cheat_assert_not_double_complex(double actual, double unexpected)`
* `cheat_assert_long_double_complex(long double actual, long double expected)`
* `cheat_assert_not_long_double_complex(long double actual, long double unexpected)`
* `cheat_assert_signed_char(signed char actual, signed char expected)`
* `cheat_assert_not_signed_char(signed char actual, signed char unexpected)`
* `cheat_assert_unsigned_char(unsigned char actual, unsigned char expected)`
* `cheat_assert_not_unsigned_char(unsigned char actual, unsigned char unexpected)`
* `cheat_assert_signed_char(signed char actual, signed char expected)`
* `cheat_assert_not_signed_char(signed char actual, signed char unexpected)`
* `cheat_assert_unsigned_char(unsigned char actual, unsigned char expected)`
* `cheat_assert_not_unsigned_char(unsigned char actual, unsigned char unexpected)`
* `cheat_assert_pointer(void const* actual, void const* expected)`
* `cheat_assert_not_pointer(void const* actual, void const* unexpected)`
* `cheat_assert_string(char const* actual, char const* expected)`
* `cheat_assert_not_string(char const* actual, char const* expected)`

This is for working around extension issues.

* `CHEAT_NO_MATH`

These are the stable parts of the internals.

* `CHEAT_H`
* `CHEATS_H`
* `CHEAT_WRAP(name)`
* `size_t CHEAT_PASS`

### 7.4   Internal Things

These are not to be relied on.

* `CHEAT_MODERN`
* `CHEAT_WINDOWED`
* `CHEAT_POSIXLY`
* `CHEAT_VERY_POSIXLY`
* `CHEAT_POSTMODERN`
* `CHEAT_GNUTIFUL`
