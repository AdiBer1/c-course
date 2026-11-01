# Your own Makefile for the "hello" program. See the task README.
#
# Requirements:
#   - `make` (inside src/) builds the program ./hello
#   - each .c file is first compiled to its own .o file (main.o, mathx.o),
#     then the .o files are linked into hello
#   - if mathx.h changes, the files that include it are rebuilt
#   - `make clean` deletes hello and all .o files
#
# Remember: the command line of a rule starts with a TAB, not spaces.

CC     = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
OBJS   = main.o mathx.o

hello: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

main.o: main.c mathx.h
	$(CC) $(CFLAGS) -c main.c -o main.o

mathx.o: mathx.c mathx.h
	$(CC) $(CFLAGS) -c mathx.c -o mathx.o

.PHONY: clean
clean:
	rm -f hello $(OBJS)