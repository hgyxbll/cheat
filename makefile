CFLAGS=-I .

build: example

clean:
	$(RM) example

test: build
	./example -s
	./example -d

example: cheat.h example.c
	$(CC) $(CFLAGS) \
		-o example example.c

.PHONY: build clean test
