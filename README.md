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

	[user@computer ~]$ wget http://github.com/Tuplanolla/cheat/raw/stable/cheat.h

 and move it to a suitable location like the global search path

	[user@computer ~]$ sudo mv -i cheat.h /usr/include

 or the working directory of your project.

	[user@computer ~]$ mv -i cheat.h project

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
You can define tests with `CHEAT_TEST(name, statements)` and
 their success conditions called assertions with `cheat_assert(bool expected)`.
Doing so is demonstrated in the example file.

	[user@computer project]$ wget http://github.com/Tuplanolla/cheat/raw/stable/example.c
	[user@computer project]$ mv -i example.c tests.c

The example also shows how you can declare global variables with
 `CHEAT_DECLARE(declarations)` and manage them with
 `CHEAT_SET_UP(statements)` and `CHEAT_TEAR_DOWN(statements)`, which
 are executed before and after each test respectively.

A detailed explanation of the entire interface is in section 3.2.
However it is important to note that the state of all global variables that
 are mutable (as in modified by any execution path) is undefined when
 the set up or, in case there is no set up, a test begins.

It is time to run your new tests.

### 1.3   Running Tests

Tests compile into an executable

	[user@computer project]$ gcc -I . -o tests tests.c

 that takes care of running the tests and reporting their outcomes.
There are two things that need to be taken care of when compiling a test suite.
First you have to add the directory of the test suite to the search path, as
 is done here with `-I .`.
Then you have to make `__BASE_FILE__` point to the test suite, by
 using `-D __BASE_FILE__=\"tests.c\"` or such, if the compiler does not.
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

The results are reported in five parts.

The first part is the progress bar, where

* a success is a green dot,
* a failure is a red colon,
* a crash is a red exclamation mark,
* a time out is a yellow exclamation mark and
* an ignored outcome is signaled with a yellow question mark.

The second part contains diagnostic messages in
 a format similar to what many popular C compilers produce.

The third and fourth part, which is omitted here, holds the contents of
 the captured standard output and error streams.

The fifth and last part, which is always shown, briefly summarizes what
 the outcome of the test suite is.
A test suite is considered successful if and only if
 every single one of its tests completes without failing a single assertion.
The outcome is also reflected by the exit code of the process.

	[user@computer project]$ echo returned $?
	returned 1

You can further change the behavior of a test suite with command line options.

	[user@computer project]$ ./tests --help

They are presented in section 3.3.

## 2   Overview

### 2.1   License

CHEAT is free software and
 as such licensed under the simplified BSD license with two clauses.
The full license can be found in the `LICENSE` file that
 resides in the same directory as this file.
In short, copies and derivative works are permitted as long as
 they use the same license.
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

### 2.4   Correctness

Everything in the project is built with extreme care and attention to detail.
While the authors are quite confident the project is free of serious bugs,
 they are mere mortals and not well versed in certified programming.

### 2.5   Contributing

The support for Windows and other more exotic operating systems is not complete.
For example stream capturing is currently very limited without POSIX interfaces.

Contributions in the forms of feedback and pull requests are all very welcome!

## 3   Advanced Usage

### 3.1   Files

The project contains other useful files in addition to the main header.
You can acquire them by cloning the repository

	[user@computer ~]$ git clone git@github.com:Tuplanolla/cheat.git
	[user@computer ~]$ cd cheat

 or downloading the clone directly.

	[user@computer ~]$ wget http://github.com/Tuplanolla/cheat/archive/master.zip
	[user@computer ~]$ unzip master.zip
	[user@computer ~]$ mv -i cheat-master cheat

In addition to the main header there is an extension header and
 examples of how to use them with various compilers.

	[user@computer cheat]$ less example.c examples.c
	[user@computer cheat]$ make -f makefile.gcc

The extensions are introduced in section 3.4.

There are also tests for corner cases,

	[user@computer cheat]$ ls tests
	[user@computer cheat]$ ./test

 supplementary reading material

	[user@computer cheat]$ man ./cheat.7
	[user@computer cheat]$ sudo cp -i cheat.7 /usr/share/man/man7
	[user@computer cheat]$ sudo gzip /usr/share/man/man7/cheat.7

 and things used during development.

	[user@computer cheat]$ xdot streams.dot
	[user@computer cheat]$ tcc -run meta.c 4
	[user@computer cheat]$ rm -i windowed.h

### 3.2   Programming Interface

Tests can be defined with `CHEAT_TEST(name, statements)`,
 where `name` must be a valid identifier and `statements` a list of statements.
The identifier must not conflict with an existing preprocessor directive.
For example `putc` is not a valid identifier, because
 it is reserved by the standard library and
 `exit` and `write` may not be valid unless `CHEAT_NO_WRAP` is defined, because
 procedures that resemble continuations or effects are wrapped by default.
The list of statements must not be empty or ambiguous.
For example `int x, y;` may be interpreted as an invalid parameter list if
 the compiler does not support `__VA_ARGS__`.
Solutions to that are presented in section 5.5.

Tests can also be defined with
 `CHEAT_IGNORE(name, statements)` and `CHEAT_SKIP(name, statements)`.
They work like `CHEAT_TEST(name, statements)` with the exception that
 the outcome of the former is ignored and the latter is not executed at all.
One could think of them as checked comments.

(Insert an explanation of `CHEAT_DECLARE(declarations)`,
 `CHEAT_SET_UP(statements)` and
 `CHEAT_TEAR_DOWN(statements)` here.)

(Mention `cheat_assert(bool expected)` and `cheat_assert_not(bool unexpected)`.)

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

(Make this section less terrible.)

### 3.4   Extensions

There are things like `cheat_assert_not_long_double_complex(long double actual, long double unexpected)`.

(Pull things from section 7.)

### 3.5   Design Decisions

Empty tests and test suites are both have successful outcomes, because
 every predicate is true for the empty set, so why not choose favorably?

(Explain more things.)

## 4   Portability

### 4.1   Standards Compliance

CHEAT follows the original language specification, ISO/IEC 9899:1990, and
 the first POSIX specification, IEEE Std 1003.1-1988, to the letter.
It also takes the newer revisions, both
 ISO/IEC 9899:1999 and IEEE Std 1003.1-2001, into account whenever possible.

The project does not require a POSIX system to work, but it helps, because
 only the most critical features are universal and
 just a few of the rest are compatible with Windows.

While the project does not rely on a particular compiler,
 it is easier to use with some of them.
Specialized build automation scripts are provided to
 help hammer out common problems and save you from needless frustration.

	[user@computer cheat]$ make -f makefile.gcc

	computer# make -f makefile.tcc

	E:\CHEAT> makefile.bat

You can see screenshots of them in section 6.

### 4.2   Language Compatibility

The project is designed for C, but also works with C++.
It abides by ISO/IEC 14882:1998 as far as is reasonable.

Hopefully it is not an issue to wade through a million warnings.

	[user@computer cheat]$ make -e CC=g++ -f makefile.gcc

## 5   Bugs and Limitations

CHEAT is naturally fickle, because it is built with heavy preprocessor abuse.
Some problems are impossible to fix, so they are collected into this section.

### 5.1   Identifiers

Identifiers starting with `CHEAT_` and `cheat_` are reserved for
 internal use as C does not have namespaces.

(Rewrite the rest.)

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

## 6   Screenshots

Everyone likes pretty pictures.

Here is CHEAT being compiled with the GNU Compiler Collection and
 run in the Xfce terminal emulator provided by a Linux distribution.

![Screenshot](http://raw.github.com/Tuplanolla/cheat/master/xfce.png)

Here is CHEAT being compiled with Microsoft C/C++ and
 run in the command prompt of Windows XP.

![Another Screenshot](http://raw.github.com/Tuplanolla/cheat/master/xp.png)

Here is CHEAT being compiled with Borland Turbo C and
 run in the default shell of FreeDOS.

![Yet Another Screenshot](http://raw.github.com/Tuplanolla/cheat/master/dos.png)

## 7   Reference

### 7.1   Compilation Things

These form the primary interface.

* `CHEAT_TEST(name, statements)`
* `CHEAT_DECLARE(declarations)`
* `CHEAT_SET_UP(statements)`
* `CHEAT_TEAR_DOWN(statements)`
* `CHEAT_IGNORE(name, statements)`
* `CHEAT_SKIP(name, statements)`

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
