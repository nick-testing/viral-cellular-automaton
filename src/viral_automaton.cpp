#include "viral_automaton.h"
#include <iostream>             /* cout */
#include <random>               /* srand, rand */
#include <time.h>               /* time */
#include <vector>               /* vector */
#include <tuple>                /* tuple */
#include <unistd.h>             /* sleep */              
#include <fstream>

using std::vector, std::tuple, std::get, std::cin, std::cout, std::endl;

#define SLEEP_TIME 200000


// Global variable counters
static int N = 30000;               // Total number of occupied cells
static double D = 0.002;            // Fraction of starting sick cells
static double R = 0.01;             // Fraction of fast moving cells
static int P1 = 60;                 // Probability (percentage) of people getting sick when hygiene is not maintained
static int P2 = 5;                  // Probability (percentage) of people getting sick when hygiene is maintained
static int T;                       // P1 to P2 threshold - 0.1 * N Default
static int sick_counter = 0;

static std::random_device rd;

/* Externally accessible variables */
int _tmp_sick_counter = 0;          // Will store mid-iteration sick number of each iteration.
int _healthy_counter = 0;
int _immune_counter = 0;
int _gen_to_immune = 2;             // Generations until sick cells become immune

vector<Cell*> cell_array;           // vector that stores pointers to all occupied cells.

matrixData adj_mat[MATRIX_SIZE][MATRIX_SIZE];

/**
 * Checks for free cells on the grid, returns a random (x, y) coordinate as a tuple.
 */
tuple<int, int> get_random_coordinates() {
    int x;
    int y;
    std::uniform_int_distribution<int> distribution(0, 199);
    std::mt19937 engine(rd()); // Mersenne twister MT19937
    while (true) {
        x = distribution(engine);
        y = distribution(engine);
        if (!adj_mat[x][y].is_occupied) {
            break;
        }
    }
    return std::tuple<int, int>(x, y);
}

/*
 * Selects based on a given probability whether a cell is sick in the next iteration
 */
bool is_cell_sick(Cell* cell) {
    // Create a uniform distribution number generator
    std::uniform_int_distribution<int> distribution(1, 100);
    std::mt19937 engine(rd()); // Mersenne twister MT19937
    int value = distribution(engine);

    auto neighbors = cell->get_neighbours(0);
    int probability = 0;
    // Sick amount is below threshold
    if (sick_counter < T) {
        for (auto neighbor : neighbors) {
            int x = get<0>(neighbor);
            int y = get<1>(neighbor);
            if (adj_mat[x][y].is_occupied && adj_mat[x][y].is_sick) {
                probability += P1;
            }
        }
    } else {
        for (auto neighbor : neighbors) {
            int x = get<0>(neighbor);
            int y = get<1>(neighbor);
            if (adj_mat[x][y].is_occupied && adj_mat[x][y].is_sick) {
                probability += P2;
            }
        }
    }
    if (probability >= 100)
        return true;
    else
        return value < probability; // will be sick probability/100 times.
}

/**
 * This function ensures that each cell will only travel to an unoccupied cell in the following generation.
 * Selects a random neighbor coordinate from the surrounding cells(either neighbours with distance of 1, or neighbors
 * with distance of 10, depending on the cell, including the cell itself.)
 * Will select a cell from non occupied cells only
 * @param all_neighbor_coordinates Vector containing tuples
 * @param self_coordinate tuple representing current point's coordinates
 * @return new coordinates
 */
tuple<int,int> choose_random_neighbor(vector<tuple<int, int>> all_neighbor_coordinates,
                                      tuple<int, int> self_coordinate) {     
    vector<tuple<int, int>> free_spaces;
    free_spaces.push_back(self_coordinate);
    int i = 1;
    for (tuple<int, int> neighbor_coordinate : all_neighbor_coordinates) {
        int x = get<0>(neighbor_coordinate);
        int y = get<1>(neighbor_coordinate);
        if (!adj_mat[x][y].is_occupied) {
            ++i; // holds number of free cells
            free_spaces.push_back(neighbor_coordinate);
        }
    }

    // Create a uniform distribution number generator
    std::uniform_int_distribution<int> distribution(0, i - 1);
    std::mt19937 engine(rd());
    int choice = distribution(engine);
    return free_spaces.at(choice);
}

/**
 * Clears all the flags of the requested cell.
 * @param x row number
 * @param y column number
 */
void free_cell(int x, int y) {
    adj_mat[x][y].is_occupied = false;
    adj_mat[x][y].is_sick = false;
    adj_mat[x][y].is_healthy = false;
    adj_mat[x][y].is_immune = false;
    adj_mat[x][y].cell = nullptr;
}

/**
 * Read and initialize automata parameters interactively
 */
static void get_params() {
    cout << "=========================================Viral Spread Automaton"
            "=========================================" 
        <<  endl << endl 
        <<  "\033[1;41;37mNOTE: after parameter setup, it is highly recommended to set the terminal"
        <<  endl
        <<  "font size to 5 or lower, and the terminal size to at least 201x201"
        <<  endl
        <<  "in order to see the whole simulation.\033[0m"
        <<  endl << endl;

    while(true) {
        cout << "Enter desired number of cells, maximum of 40000 [Default = 30000]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        } else if (!(std::cin >> N) || N > 40000 || N < 2) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input, please try again." << endl;     //error handling
        } else {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    while(true) {
        cout << "Enter fraction of sick cells, number between 0.0-1.0 [Default = 0.002]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            D = 0.002;
            break;
        } else if (!(std::cin >> D) || (D > 1.0 || D < 0.0)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input, please try again." << endl;     //error handling
        } else {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    while(true) {
        cout << "Enter number of fast moving cells, value should be between 0.0-1.0 [Default = 0.01]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            R = 0.01;
            break;
        } else if (!(std::cin >> R) || (R > 1.0 || R < 0.0)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input, please try again." << endl;     //error handling
        } else {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    while(true) {
        cout << "Enter number of generations until a sick cell stops infecting [Default = 2]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            _gen_to_immune = 2;
            break;
        } else if (!(std::cin >> _gen_to_immune) || (_gen_to_immune < 1)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input, please try again." << endl;     //error handling
        } else {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    while(true) {
        cout << "Enter precentile probability of infection(values 1 - 100)"
                " when number of sick cells is low [Default = 60]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            P1 = 60;
            break;
        } else if (!(std::cin >> P1) || (P1 < 1 || P1 > 100)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input, please try again." << endl;     //error handling
        } else {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    while(true) {
        cout << "Enter percentile probability of infection( values 1 - 100)"
                " when number of sick cells is high [Default = 5]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            P2 = 5;
            break;
        } else if (!(std::cin >> P2) || (P2 < 1 || P2 > 100)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input, please try again." << endl;     //error handling
        } else {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    double x;
    while(true) {
        cout << "Enter sick cell threshold before cell infection percentage drops,"
                        " values should be between 0.0-1.0 [Default = 0.1]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            T = 0.1 * N;
            break;
        } else if (!(std::cin >> x) || (x < 0.0 || x > 1.0)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input, please try again." << endl;     //error handling
        } else {
            T = x * N;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    cout << endl << "Parameters initialized! press Enter to start the simulation.";
    if (cin.peek() == '\n') {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
}

/****************************************************************************
 ****************************** Console Output ******************************
 ****************************************************************************/

/**
 * Clears the console display
 */
static void clear() {
    #if defined _WIN32
    system("cls");
    #elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__) || defined (__APPLE__)
    system("clear");
    #endif
}

static void print_matrix(matrixData adj_mat[MATRIX_SIZE][MATRIX_SIZE]) {
    int i = 0, j = 0;
    //iterate over rows
    for (;i < MATRIX_SIZE; ++i) {
        //iterate over columns
        for (j = 0; j < MATRIX_SIZE; ++j) {
            if (!adj_mat[i][j].is_occupied)
                cout <<"_";
            else if (adj_mat[i][j].is_occupied && adj_mat[i][j].is_healthy)
               cout <<"\033[1;47;35mH\033[0m";
            else if (adj_mat[i][j].is_occupied && adj_mat[i][j].is_sick)
                cout <<"\033[1;41;37mS\033[0m";
            else if (adj_mat[i][j].is_occupied && adj_mat[i][j].is_immune)
                cout << "\033[1;104;30mI\033[0m";
        }
        cout << endl;
    }
    //leave matrix on the screen for a couple of seconds
    usleep(SLEEP_TIME);
}

/****************************************************************************
 ************************** Execution Functions *****************************
 ****************************************************************************/

/**
 * Initialize matrix with sick, healthy and long distance cells
 */
static void initialize() {
    int i = 0;
    int j = 0;
    int num_of_healthy =  N - N * D;
    int fast_cells = N * R;
    // Initialize a random amount of sick and healthy long distance cells
    int fast_healthy_cells = rand() % fast_cells;
    int fast_sick_cells = fast_cells - fast_healthy_cells;
    // Loop initializing healthy
    for (; i < num_of_healthy; ++i) {
        auto coor = get_random_coordinates();
        if (i > fast_healthy_cells) {
            cell_array.push_back(new HealthyCell(coor, 0));
        } else {
            cell_array.push_back(new HealthyCell(coor, 9));
        }
    }
    _healthy_counter = num_of_healthy; // initial number of healthy
    int numSick = N - num_of_healthy;
    sick_counter = numSick; // initial counter of sick
    // Loop initializing sick
    for (; j < numSick; ++j) {
        auto coor = get_random_coordinates();
        if (j > fast_sick_cells) {
            cell_array.push_back(new SickCell(coor, 0));
        } else {
            cell_array.push_back(new SickCell(coor, 9));
        }
    }
}

// Deletes cell vector from memory
static void clear_memory(vector<Cell*> cellArray) {
    for (Cell* cell: cellArray) {
        delete cell;
    }
}

void execute() {
    srand(time(NULL));
    clear();
    get_params();
    initialize();
    int iteration = 1000;
    int i = 0;
    int j = 0;
    std::ofstream output_file("results.csv");
    output_file << "Healthy,Sick,Immune" << endl;
    for (; i < iteration; ++i) {
        j = 0;
        clear();
        cout << "\033[3;43;30mIteration\033[0m ";
        printf("\033[3;43;30m%d\033[0m\n", i);
        print_matrix(adj_mat);
        _tmp_sick_counter = sick_counter;

        output_file << _healthy_counter << "," << sick_counter << "," << _immune_counter << endl;

        if (sick_counter > 0) {
            // Iterate over each and every cell in the cell vector, advancing one cell at a time,
            // and ensuring that there will be no cell overlapping.
            for (auto cell : cell_array) {
                cell -> next_iteration(j);
                ++j;
            }
        } else
            break;
        sick_counter = _tmp_sick_counter; // update sick_counter with new value
    }
    output_file.close();
    cout << endl << "Results were written to results.csv" << endl;
    clear_memory(cell_array);
}