CC=g++
CFLAGS= -std=c++17 -c -Wall -O3
SRC=./src/
OBJECTS = $(SRC)viral_automaton.o $(SRC)classes.o $(SRC)main.o

all: viral-automaton.out

viral-automaton.out: $(OBJECTS)
	$(CC) $(OBJECTS) -o viral-automaton.out

 %.o: $(SRC)%.cpp
	$(CC) $(CFLAGS) $< -o $(SRC)

clean:
	rm -f viral-automaton.out results.csv $(SRC)*.o

