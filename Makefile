CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -pedantic -ggdb
LIBS=
INPUT=

.PHONY: test

test: colt $(INPUT)
	./colt $(INPUT)

colt: colt.c
	$(CC) $(CFLAGS) -o colt colt.c $(LIBS)