#include "classes.h"

using namespace std;

matrixData adj_mat[200][200];


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

