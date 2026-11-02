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

hello: main.o mathx.o
	gcc main.o mathx.o -o hello

main.o: main.c mathx.h
	gcc -c main.c -o main.o

mathx.o: mathx.c mathx.h
	gcc -c mathx.c -o mathx.o

clean:
	rm -f main.o mathx.o hello

.PHONY: clean