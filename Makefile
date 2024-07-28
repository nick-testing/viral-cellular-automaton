main:
	g++ -std=c++17 ./src/viral_automaton.cpp ./src/classes.cpp -o viral-automaton.out
clean:
	rm -f viral-automaton.out results.csv

