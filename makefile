CFLAGS=-I .
LDLIBS=-lm

build: example examples

clean:
	$(RM) example examples

test: build
	- ./example -s
	- ./examples -s

example: cheat.h example.c
	$(CC) $(CFLAGS) \
		-o example example.c

examples: cheat.h cheats.h examples.c
	$(CC) $(CFLAGS) \
		-o examples examples.c $(LDLIBS)

.PHONY: build clean test
