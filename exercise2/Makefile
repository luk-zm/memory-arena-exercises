all: bin bin/arena.o bin/test.o
	gcc bin/arena.o bin/test.o -o bin/arena-test

bin/arena.o: arena.h arena.c
	gcc -c arena.c -o bin/arena.o

bin/test.o: test.c
	gcc -c test.c -o bin/test.o

bin:
	@mkdir -p bin
