CC=gcc
CFLAGS= -g -Wall

ALL: json

json: json.c
	$(CC) -o $@ $^ $(CFLAGS)

clean: json
	rm $^