#include "classes.h"
#include "viral_automaton.h"

using std::vector, std::tuple, std::get;

extern vector<Cell*> cell_array;    // vector that stores pointers to all occupied cells.
extern int GEN_TO_IMMUNE;       // Generations until sick cells become immune
extern int healthy_counter;
extern int temp_sick_counter;    
extern int immune_counter;    

extern matrixData adj_mat[200][200];


Cell::Cell(tuple<int, int> coor, int speed): location(coor), speed(speed){};

HealthyCell::HealthyCell(tuple<int, int> coor, int speed): Cell(coor, speed) {
        int x = get<0>(coor);
        int y = get<1>(coor);
        adj_mat[x][y].cell = this;
        adj_mat[x][y].is_occupied = true;
        adj_mat[x][y].is_healthy = true;
    };

SickCell::SickCell(tuple<int, int> coor, int speed): Cell(coor, speed) {
    int x = get<0>(coor);
    int y = get<1>(coor);
    adj_mat[x][y].cell = this;
    adj_mat[x][y].is_occupied = true;
    adj_mat[x][y].is_sick = true;
};

SickCell::SickCell(tuple<int, int> coor, int speed, int gen): Cell(coor, speed), generation(gen) {
    int x = get<0>(coor);
    int y = get<1>(coor);
    adj_mat[x][y].cell = this;
    adj_mat[x][y].is_occupied = true;
    adj_mat[x][y].is_sick = true;
};

ImmuneCell::ImmuneCell(tuple<int, int> coor, int speed): Cell(coor, speed) {
    int x = get<0>(coor);
    int y = get<1>(coor);
    adj_mat[x][y].cell = this;
    adj_mat[x][y].is_occupied = true;
    adj_mat[x][y].is_immune = true;
};


/*****************************************************************************
 ****************************** Object Functions *****************************
 *****************************************************************************/

/**
 * Returns a vector containing neighbor coordinates for the current cell
 *  Neigbors of the middle cell are represented as:
 *  1 2 3
 *  4   5
 *  6 7 8  
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
    if (generation < GEN_TO_IMMUNE) {
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

