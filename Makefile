CC=gcc
CFLAGS= -g -Wall

ALL:encode_test parse_test

encode_test: encode_test.c json.c
	$(CC) -o $@ $^ $(CFLAGS)

parse_test: parse_test.c json.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf encode_test parse_test
