main:
	g++ -std=c++11 ./src/viral_automaton.cpp -o viral_automaton.out
clean:
	rm -f viral_automaton.out results.csv

