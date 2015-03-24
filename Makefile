CC=gcc
CFLAGS= -g -Wall

ALL:test

test: test.c json.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf test
