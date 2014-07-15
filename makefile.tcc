CC=tcc
warnings=-Wall -Wunusupported -Wwrite-strings
debug=$(warnings) -g
deploy=-w
CFLAGS=$(debug) \
	-D__STDC_VERSION__=199409L -D_POSIX_C_SOURCE=198809L \
	-I . # These refer to ISO/IEC 9899:1990/Amd 1:1995 and IEEE Std 1003.1-1988.

build: cheat-example

clean:
	$(RM) cheat-example

test: build
	- ./cheat-example -s
	- ./cheat-example -d

cheat-example: cheat.h cheat-helpers.h cheat-example.c
	$(CC) $(CFLAGS) \
		-D__BASE_FILE__=\"cheat-example.c\" -o cheat-example cheat-example.c

.PHONY: build clean test
