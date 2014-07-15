CFLAGS=-I .

build: cheat-example

clean:
	$(RM) cheat-example

test: build
	./cheat-example -s
	./cheat-example -d

cheat-example: cheat.h cheat-helpers.h cheat-example.c
	$(CC) $(CFLAGS) \
		-o cheat-example cheat-example.c

.PHONY: build clean test
