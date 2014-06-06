# CHEAT

CHEAT is a minimal unit testing framework for the C programming language.
Its primary use cases are small projects and systems for embedded platforms.
It has no dependencies and requires no installation or configuration.
The design philosophy is that time spent not writing tests is time wasted.
Only a header file and a statement block is needed.

    #include <cheat.h>

    TEST(mathematics_still_work, {
        cheat_assert(2 + 2 == 4);
        cheat_assert(2 + 2 != 5);
    })

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
    [user@computer /usr/include]$ sudo wget -q https://github.com/Tuplanolla/cheat/blob/master/cheat.h

### Writing Tests

Tests should be put in their own source file,

    [user@computer ~/project]$ cat > tests.c

 which has to include `cheat.h`

    #include <cheat.h>

 and define `__BASE_FILE__`

    #ifndef __BASE_FILE__
    #define __BASE_FILE__ __FILE__
    #endif

 if the compiler does not (for example GCC does, but
 use it anyway for portability).

Tests can then be defined with `TEST(name, block)`,
 global variables with `GLOBALS(declarations)`,
 initialization with `SET_UP(block)` and
 finalization with `TEAR_DOWN(block)`.

Examples are in the `cheat-example.c` file, which
 can be compiled with `make` and run with `make test`.

### Running Tests

Tests compile into an executable

    [user@computer ~/project]$ gcc -o tests tests.c

 that takes care of running the tests and reporting their outcomes.

    [user@computer ~/project]$ ./tests
    .F..S...
    tests.c:42: Assertion failed: '2 + 2 == 5'.
    2 failed tests of 8 tests run.

The executable runs tests in
 an isolated subprocess, so
 the framework does not crash if
 one of the tests does.
The `--no-fork` flag allows
 running everything in the same process if
 `fork()` is unsupported, fails or
 something else equally fun happens.

### Complicated Tests

More complicated tests are easy to define with
 the assistance of CHEAT helpers that
 exist in `cheat-helpers.h`.

    #include <cheat-helpers.h>

Helpers cover things like
 stream redirection and
 signal handlers.

## Bugs and Limitations

CHEAT is naturally fickle, because
 it is built on preprocessor abuse.
Some problems that are impossible to fix are
 collected into the list below.

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
