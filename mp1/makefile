# makefile

all: memtest

ackerman.o: ackerman.c ackerman.h 
	gcc -Wall -std=c99 -c -g ackerman.c

my_allocator.o: my_allocator.c my_allocator.h
	gcc -Wall -std=c99 -c -g my_allocator.c

error.o: error.c
	gcc -Wall -std=c99 -c -g error.c

memtest: memtest.c ackerman.o my_allocator.o error.o 
	gcc -o memtest memtest.c ackerman.o my_allocator.o error.o -lm

def:
	./memtest

test:
	./memtest -s 1048576 -b 1024

clean:
	rm *.o

