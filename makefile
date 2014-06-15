size=`getconf PAGESIZE`
warnings=`cat gcc-4_7_0-release` -Wno-error -Wno-system-headers \
		-Wno-c++-compat -Wno-traditional -Wno-traditional-conversion \
		-Wformat-nonliteral
debug=-g -O0 $(warnings)
deploy=-s -O3
CFLAGS=-std=c89 -D__STDC_VERSION__=199409L \
		-D_POSIX_C_SOURCE=200112L -D_XOPEN_SOURCE=600 \
		-I . \
		$(debug) # actually -D__STDC_VERSION__=198912L

build: cheat-example

clean:
	$(RM) cheat-example
	$(RM) cheat_captured_*

test: build
	- ./cheat-example

cheat-example: cheat.h cheat-helpers.h cheat-example.c
	$(CC) $(CFLAGS) -o cheat-example cheat-example.c

.PHONY: build clean test
