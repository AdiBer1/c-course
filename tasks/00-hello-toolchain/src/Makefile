CC = gcc
CFLAGS = -Wall -Wextra -pedantic

hello: main.o mathx.o
	$(CC) main.o mathx.o -o hello

main.o: main.c mathx.h
	$(CC) $(CFLAGS) -c main.c -o main.o

mathx.o: mathx.c mathx.h
	$(CC) $(CFLAGS) -c mathx.c -o mathx.o

clean:
	rm -f hello main.o mathx.o

.PHONY: clean
