all: fork_inverter

fork_inverter.o: fork_inverter.cpp
	g++ -Wall -c fork_inverter.cpp -o fork_inverter.o

error.o: error.c
	g++ -Wall -c error.c

fork_inverter: fork_inverter.cpp error.o
	g++ fork_inverter.cpp error.o -o fork_inverter

clean:
	rm *.o