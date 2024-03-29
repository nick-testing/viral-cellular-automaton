#include "function_includes.h"


/**
 * Checks for free cells on the grid, returns a random (x, y) coordinate as a tuple.
 * @return
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
 * Rolls whether a cell is sick in the next iteration
 * Uses
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
    // Create a uniform distribution number generator


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
    std::uniform_int_distribution<int> distribution(0, i - 1);
    std::mt19937 engine(rd()); // Mersenne twister MT19937
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
 * Read and initialize automata parameters from user
 */
void get_params() {
    cout << "=========================================Viral Spread Automaton=========================================" <<
    endl << endl << endl <<
    "\033[1;41;37mNOTE: it is highly recommended to set the terminal font to 5 or lower, and the terminal size to at least 201x201 after parameter setup,\n"
                    "in order to better see the whole simulation.\033[0m"  << endl << endl;

    while(true) {
        cout << "Enter desired number of cells, maximum of 40000 [Default = 30000]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            N = 30000;
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
            X = 2;
            break;
        } else if (!(std::cin >> X) || (X < 1)) {
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
        cout << "Enter probability of infection in percents(1 - 100) when number of sick cells is low [Default = 60]: ";
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
        cout << "Enter probability of infection in percents(1 - 100) when number of sick high [Default = 5]: ";
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
        cout << "Enter sick cell threshold before cell infection percentage drops, values should be between 0.0-1.0 [Default = 0.1]: ";
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

/**
 * Initialize matrix with sick, healthy and long distance cells
 */
void initialize() {
    int i = 0;
    int j = 0;
    int num_of_healthy =  N - N * D;
    int fast_cells = N * R;
    // Initialize a random amount of sick and healthy long distance cells
    int fast_healthy_cells = rand() % fast_cells;
    int fast_sick_cells = fast_cells - fast_healthy_cells;
    // Loop initializing healthy
    for (i; i < num_of_healthy; ++i) {
        auto coor = get_random_coordinates();
        if (i > fast_healthy_cells) {
            cell_array.push_back(new HealthyCell(coor, 0));
        } else {
            cell_array.push_back(new HealthyCell(coor, 9));
        }
    }
    healthy_counter = num_of_healthy; // initial number of healthy
    int numSick = N - num_of_healthy;
    sick_counter = numSick; // initial counter of sick
    // Loop initializing sick
    for (j; j < numSick; ++j) {
        auto coor = get_random_coordinates();
        if (j > fast_sick_cells) {
            cell_array.push_back(new SickCell(coor, 0));
        } else {
            cell_array.push_back(new SickCell(coor, 9));
        }
    }
}

// Deletes cell vector from memory
void clear_memory(vector<Cell*> cellArray) {
    for (Cell* cell: cellArray) {
        delete cell;
    }
}

void execute() {
    clear();
    get_params();
    initialize();
    int iteration = 1000;
    int i = 0;
    int j = 0;
    std::ofstream output_file("results.csv");
    output_file << "Healthy,Sick,Immune" << endl;
    for (i; i < iteration; ++i) {
        j = 0;
        clear();
        cout << "\033[3;43;30mIteration\033[0m ";
        printf("\033[3;43;30m%d\033[0m\n", i);
        print_matrix(adj_mat);
        temp_sick_counter = sick_counter;

        //cout << healthy_counter << "," << sick_counter << "," << immune_counter << endl;
        output_file << healthy_counter << "," << sick_counter << "," << immune_counter << endl;

        if (sick_counter > 0) {
            // Iterate over each and every cell in the cell vector, advancing one cell at a time,
            // and ensuring that there will be no cell overlapping.
            for (auto cell : cell_array) {
                cell -> next_iteration(j);
                ++j;
            }
        } else
            break;
        sick_counter = temp_sick_counter; // update sick_counter with new value
    }
    output_file.close();
    cout << endl << "Results were written to results.csv" << endl;
}


int main() {
    srand(time(NULL));
    execute();
    clear_memory(cell_array);
}


/*****************************************************************************
 ****************************** Object Functions *****************************
 *****************************************************************************/

/**
 * Returns a vector containing neighbor coordinates for the current cell
 *  Neigbors of the middle cell are represented as:
 *  1 2 3
 *  4   5
 *  6 7 8  
 * @param radius proximity to neighbors
 * @return tuple
 */
std::vector<std::tuple<int, int>> Cell::get_neighbours(int radius) {
    std::vector<std::tuple<int, int>> output;
    int x = get<0>(location);
    int y = get<1>(location);
       
    if (x > radius && y > radius && x < 199 - radius && y < 199 - radius) {
        output.push_back(tuple<int, int>(x - 1 - radius, y - 1 - radius));      // 1
        output.push_back(tuple<int, int>(x - 1 - radius, y));                   // 2
        output.push_back(tuple<int, int>(x - 1 - radius, y + 1 + radius));      // 3
        output.push_back(tuple<int, int>(x, y - 1 - radius));                   // 4
        output.push_back(tuple<int, int>(x, y + 1 + radius));                   // 5
        output.push_back(tuple<int, int>(x + 1 + radius, y - 1 - radius));      // 6
        output.push_back(tuple<int, int>(x + 1 + radius, y));                   // 7
        output.push_back(tuple<int, int>(x + 1 + radius, y + 1 + radius));      // 8
        return output;
    }
    else if (x <= radius && x >= 0) {
        output.push_back(tuple<int, int>(199 - radius + x, y));              // 2
        output.push_back(tuple<int, int>(x + 1 + radius, y));            // 7

        // only top is wrapping
        if (y > radius && y < 199 - radius){
            output.push_back(tuple<int, int>(199 - radius + x, y - 1 - radius));  // 1
            output.push_back(tuple<int, int>(199 - radius + x, y + 1 +radius));  // 3
            output.push_back(tuple<int, int>(x, y - 1 - radius));    // 4
            output.push_back(tuple<int, int>(x, y + 1 + radius));    // 5
            output.push_back(tuple<int, int>(x + 1 + radius, y - 1 - radius));    // 6
            output.push_back(tuple<int, int>(x + 1 + radius, y + 1 + radius));    // 8
            return output;
        }

        //top and right are wrapping
        else if (y >= 199 - radius && y < 200) {
            output.push_back(tuple<int, int>(199 - radius + x, y - 1 - radius));  // 1
            output.push_back(tuple<int, int>(199 - radius + x, radius - (199 - y)));      // 3
            output.push_back(tuple<int, int>(x, y - 1 - radius));    // 4
            output.push_back(tuple<int, int>(x, radius - (199 - y)));        // 5
            output.push_back(tuple<int, int>(x + 1 + radius, y - 1 - radius));    // 6
            output.push_back(tuple<int, int>(x + 1 + radius, radius - (199 - y)));        // 8
            return output;
        }

            // top and left are wrapping.

        else if (y <= radius && y >= 0) {
            output.push_back(tuple<int, int>(199 - radius + x, 199 - radius + y));    // 1
            output.push_back(tuple<int, int>(199 - radius + x, y + 1 + radius ));      // 3
            output.push_back(tuple<int, int>(x, 199 - radius + y));      // 4
            output.push_back(tuple<int, int>(x, y + 1 + radius));        // 5
            output.push_back(tuple<int, int>(x + 1 + radius, 199 - radius + y));      // 6
            output.push_back(tuple<int, int>(x + 1 + radius, y + 1 + radius));        // 8
            return output;
        }
    }
    // bottom is wrapping
    else if (x >= 199 - radius && x < 200) {
        output.push_back(tuple<int, int>(x - 1 - radius, y));              // 2
        output.push_back(tuple<int, int>(x - 199 + radius, y));                // 7

        // only bottom row is wrapping
        if (y > radius && y < 199 - radius) {
            output.push_back(tuple<int, int>(x - 1 - radius, y - 1 - radius));  // 1
            output.push_back(tuple<int, int>(x - 1 - radius, y + 1 + radius));  // 3
            output.push_back(tuple<int, int>(x, y - 1 - radius));    // 4
            output.push_back(tuple<int, int>(x, y + 1 + radius));    // 5
            output.push_back(tuple<int, int>(x - 199 + radius, y - 1 - radius));    // 6
            output.push_back(tuple<int, int>(x - 199 + radius, y + 1 + radius));    // 8
            return output;
        }

        //bottom right is wrapping
        else if (y >= 199 - radius && y < 200) {
            output.push_back(tuple<int, int>(x - 1 - radius, y - 1 - radius));  // 1
            output.push_back(tuple<int, int>(x - 1 - radius, y - 199 + radius));      // 3
            output.push_back(tuple<int, int>(x, y - 1 - radius));  // 4
            output.push_back(tuple<int, int>(x, y - 199 + radius));      // 5
            output.push_back(tuple<int, int>(x - 199 + radius, y - 1 - radius));    // 6
            output.push_back(tuple<int, int>(x - 199 + radius, y - 199 + radius));        // 8
            return output;
        }

            // bottom left is wrapping
        else if (y <= radius && y >= 0) {
            output.push_back(tuple<int, int>(x - 1 - radius, 199 - radius + y));    // 1
            output.push_back(tuple<int, int>(x - 1 - radius, y + 1 + radius));  // 3
            output.push_back(tuple<int, int>(x, 199 - radius + y));    // 4
            output.push_back(tuple<int, int>(x, y + 1 + radius));  // 5
            output.push_back(tuple<int, int>(x - 199 + radius, 199 - radius + y));      // 6
            output.push_back(tuple<int, int>(x - 199 + radius, y + 1 + radius));        // 8
            return output;
        }
    }
    // only left is wrapping
    else if (y <= radius  && y >= 0) {
        output.push_back(tuple<int, int>(x - 1 - radius, 199 - radius + y));      // 1
        output.push_back(tuple<int, int>(x - 1 - radius, y));           // 2
        output.push_back(tuple<int, int>(x - 1 - radius, y + 1 + radius));   // 3
        output.push_back(tuple<int, int>(x, 199 - radius + y));             // 4
        output.push_back(tuple<int, int>(x, y + 1 + radius));            // 5
        output.push_back(tuple<int, int>(x + 1 + radius, 199 - radius + y));      // 6
        output.push_back(tuple<int, int>(x + 1 + radius, y));            // 7
        output.push_back(tuple<int, int>(x + 1 + radius, y + 1 + radius));     // 8
        return output;
    }
    // only right is wrapping
    else if (y >= 199 - radius && y < 200) {
        output.push_back(tuple<int, int>(x - 1 - radius, y - 1 - radius));      // 1
        output.push_back(tuple<int, int>(x - 1 - radius, y));           // 2
        output.push_back(tuple<int, int>(x - 1 - radius, y - 199 + radius));   // 3
        output.push_back(tuple<int, int>(x, y - 1 - radius));             // 4
        output.push_back(tuple<int, int>(x, y - 199 + radius));            // 5
        output.push_back(tuple<int, int>(x + 1 + radius, y - 1 - radius));      // 6
        output.push_back(tuple<int, int>(x + 1 + radius, y));            // 7
        output.push_back(tuple<int, int>(x + 1 + radius, y - 199 + radius));     // 8
        return output;
    } else
        output.push_back(get_random_coordinates());
    return output;
}


/**
 * Performs one iteration step for the healthy cell that was called from.
 * @param index
 */
void HealthyCell::next_iteration(int index) {
    int x = get<0>(location);
    int y = get<1>(location);

    bool sick_next_iteration = is_cell_sick(this);
    auto all_neighbor_coordinates = get_neighbours(speed);
    auto new_cell_coordinate = choose_random_neighbor(all_neighbor_coordinates, location);

    // Freeing up current location, so that another cell can occupy it.
    free_cell(x, y);
    // create a new Healthy cell
    if (!sick_next_iteration) {
        cell_array.at(index) = new HealthyCell(new_cell_coordinate, speed);
    }
    // Create a new sick cell
    else {
        healthy_counter--;
        temp_sick_counter++;
        cell_array.at(index) = new SickCell(new_cell_coordinate, speed);
    }
    delete this;
}

void SickCell::next_iteration(int index) {
    auto all_neighbor_coordinates = get_neighbours(speed);
    auto new_cell_coordinate = choose_random_neighbor(all_neighbor_coordinates, location);
    int x = get<0>(location);
    int y = get<1>(location);
    // Freeing up current location, so that another cell can occupy it.
    free_cell(x, y);
    if (generation < X) {
        cell_array.at(index) = new SickCell(new_cell_coordinate, speed, generation + 1);
    } else {
        cell_array.at(index) = new ImmuneCell(new_cell_coordinate, speed);
        temp_sick_counter--;
        immune_counter++;
    }
    delete this;
}

void ImmuneCell::next_iteration(int index) {
    auto all_neighbor_coordinates = get_neighbours(speed);
    auto new_cell_coordinate = choose_random_neighbor(all_neighbor_coordinates, location);
    int x = get<0>(location);
    int y = get<1>(location);
    // Freeing up current location, so that another cell can occupy it.
    free_cell(x, y);
    cell_array.at(index) = new ImmuneCell(new_cell_coordinate, speed);
    delete this;
}



/****************************************************************************
 ****************************** Console Output ******************************
 ****************************************************************************/

/**
 * Clears the console display
 */
void clear() {
#if defined _WIN32
    system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__) || defined (__APPLE__)
    system("clear");
    //std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequence
#endif
}


void print_matrix(matrixData adj_mat[200][200]) {
    int i = 0, j = 0;
    //iterate over rows
    for (;i < 200; ++i) {
        //iterate over columns
        for (j = 0; j < 200; ++j) {
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
}
