# Virus Spread Simulation Cellular Automaton
A program simulating a viral outbreak and resembling [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) written in C++.

The program allows interactive parameter tweaking (such as starting number of cells, infection rates etc.) on startup.

A simple CLI UI for graphical display of the simulation is implemented.

### Prerequisites
  - g++/c++ compiler
  - Linux OS

### Running the program
   - `git clone` the repository
   - A makefile was provided for conveniece, run `make` to compile the source code, a viral-automaton.out file will appear.
   - In order to execute run `./viral-automaton.out` and follow the on screen instructions
      - After parameter initalization, it is highly reccommended to set the terminal window to 201 rows and 200 columns, in order to fully see the automation.
   - To delete the executable and the results file, run `make clean`
