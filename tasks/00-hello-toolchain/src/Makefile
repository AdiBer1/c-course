CC = gcc
CFLAGS = -Wall -Wextra

hello: main.o mathx.o
	$(CC) $(CFLAGS) -o hello main.o mathx.o

main.o: main.c mathx.h
	$(CC) $(CFLAGS) -c main.c

mathx.o: mathx.c mathx.h
	$(CC) $(CFLAGS) -c mathx.c

clean:
	rm -f hello *.o

.PHONY: clean