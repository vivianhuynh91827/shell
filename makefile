all: shell.c
  gcc -o program shell.c

run:
        ./program

clean:
        rm *.o
        rm program
