CC=tcc
warnings=-Wall -Wunusupported -Wwrite-strings
debug=$(warnings) -g
deploy=-w
CFLAGS=$(debug) \
	-D_POSIX_C_SOURCE=200112L -D_XOPEN_SOURCE=600 \
	-I . # actually -D__STDC_VERSION__=198912L

build: cheat-example

clean:
	$(RM) cheat-example
	$(RM) cheat_captured_*

test: build
	- ./cheat-example

cheat-example: cheat.h cheat-helpers.h cheat-example.c
	$(CC) $(CFLAGS) \
		-D__BASE_FILE__=\"cheat-example.c\" -o cheat-example cheat-example.c

.PHONY: build clean test
