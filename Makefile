all: main.o matrix.o
	gcc -Wall -std=c99 -o main main.o matrix.o

matrix.o: matrix.h matrix.c
	gcc -Wall -std=c99 -c matrix.h matrix.c

main.o: main.c matrix.h
	gcc -Wall -std=c99 -c main.c

clean:
	rm *.o main *.gch
