all: shell.o parse.o execute.o
	gcc -o program shell.o parse.o execute.o

shell.o: shell.c shell.h parse.h execute.h
	gcc -c shell.c

parse.o: parse.c parse.h
	gcc -c parse.c

execute.o: execute.c execute.h
	gcc -c execute.c

run:
	./program

clean:
	rm *.o
	rm program
