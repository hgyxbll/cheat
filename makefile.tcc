CC=tcc
warnings=-Wall -Wunusupported -Wwrite-strings
debug=$(warnings) -g
deploy=-w
CFLAGS=$(debug) \
	-D __STDC_VERSION__=199409L -D _POSIX_C_SOURCE=198809L \
	-I . # These refer to ISO/IEC 9899:1990/Amd 1:1995 and IEEE Std 1003.1-1988.

build: example examples

clean:
	$(RM) example examples

test: build
	- ./example -s
	- ./examples -s

example: cheat.h example.c
	$(CC) $(CFLAGS) \
		-D "__BASE_FILE__=\"example.c\"" -o example example.c

examples: cheat.h cheats.h examples.c
	$(CC) $(CFLAGS) \
		-D "__BASE_FILE__=\"examples.c\"" -o examples examples.c

.PHONY: build clean test
