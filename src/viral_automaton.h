#ifndef VIRAL_AUTOMATOH_H
#define VIRAL_AUTOMATOH_H

#include <tuple>
#include <vector>
#include "classes.h"

#define MATRIX_SIZE 200

void execute();
std::tuple<int, int> get_random_coordinates();
bool is_cell_sick(Cell* cell);
std::tuple<int,int> choose_random_neighbor(std::vector<std::tuple<int, int>>&, std::tuple<int, int>&);
void free_cell(int, int);

#endif // VIRAL_AUTOMATOH_H
