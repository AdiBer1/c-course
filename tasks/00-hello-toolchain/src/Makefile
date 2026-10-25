hello: main.o mathx.o
	gcc -o hello main.o mathx.o

main.o: main.c mathx.h
	gcc -c main.c

mathx.o: mathx.c mathx.h
	gcc -c mathx.c

clean:
	rm -f hello main.o mathx.o