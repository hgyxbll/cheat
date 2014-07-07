#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#endif

#include <cheat.h>

#ifdef _WIN32

#include <windows.h>

CHEAT_DECLARE(
	static void sleep(int const seconds) {
		Sleep(seconds * 1000);
	}
)

#elif _POSIX_C_SOURCE >= 200112L

#include <unistd.h>

#else

CHEAT_DECLARE(
	static void sleep(int const seconds) {}
)

#endif

CHEAT_TEST(sleep, {
	sleep(1);
})

CHEAT_TEST(relax, {
	sleep(1);
})

CHEAT_TEST(nap, {
	sleep(1);
})
