CFLAGS=-I .

build: cheat-example

clean:
	$(RM) cheat-example
	$(RM) cheat_captured_*

test: build
	- ./cheat-example

cheat-example: cheat.h cheat-helpers.h cheat-example.c
	$(CC) $(CFLAGS) -o cheat-example cheat-example.c

.PHONY: build clean test
