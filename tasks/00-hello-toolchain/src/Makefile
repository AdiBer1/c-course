hello: main.o mathx.o
	gcc main.o mathx.o -o hello

main.o: main.c mathx.h
	gcc -Wall -Wextra -c main.c -o main.o

mathx.o: mathx.c mathx.h
	gcc -Wall -Wextra -c mathx.c -o mathx.o

clean:
	rm -f hello main.o mathx.o
