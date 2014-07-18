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

CHEAT_TEST(sleep,
	sleep(3); /* This could be an infinite loop. */
)
