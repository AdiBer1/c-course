CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -pedantic -Wshadow -g -fsanitize=address,undefined -fsanitize-recover=address,undefined -fno-omit-frame-pointer

hello: main.o mathx.o
	$(CC) $(CFLAGS) main.o mathx.o -o hello

main.o: main.c mathx.h
	$(CC) $(CFLAGS) -c main.c -o main.o

mathx.o: mathx.c mathx.h
	$(CC) $(CFLAGS) -c mathx.c -o mathx.o

clean:
	rm -f *.o hello