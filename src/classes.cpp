#include "include.h"

using namespace std;

class Cell {
public:
    tuple<int, int> location;
    int speed;
    Cell(tuple<int, int> coor, int speed): location(coor), speed(speed){};
    std::vector<std::tuple<int, int>> get_neighbours(int radius);
    virtual void next_iteration(int index)=0;
};


struct matrixData {
    Cell* cell = nullptr;
    bool is_occupied = false;
    bool is_healthy = false;
    bool is_sick = false;
    bool is_immune = false;
};

matrixData adj_mat[200][200];


class HealthyCell : public Cell {
public:
    HealthyCell(tuple<int, int> coor, int speed): Cell(coor, speed) {
        int x = get<0>(coor);
        int y = get<1>(coor);
        adj_mat[x][y].cell = this;
        adj_mat[x][y].is_occupied = true;
        adj_mat[x][y].is_healthy = true;
    };
    void next_iteration(int index) override;

};

class SickCell : public Cell{
    int generation = 1;
public:
    SickCell(tuple<int, int> coor, int speed): Cell(coor, speed) {
        int x = get<0>(coor);
        int y = get<1>(coor);
        adj_mat[x][y].cell = this;
        adj_mat[x][y].is_occupied = true;
        adj_mat[x][y].is_sick = true;
    };
    SickCell(tuple<int, int> coor, int speed, int gen): Cell(coor, speed), generation(gen) {
        int x = get<0>(coor);
        int y = get<1>(coor);
        adj_mat[x][y].cell = this;
        adj_mat[x][y].is_occupied = true;
        adj_mat[x][y].is_sick = true;
    };
    void next_iteration(int index) override;
};

class ImmuneCell : public Cell {
public:
    ImmuneCell(tuple<int, int> coor, int speed): Cell(coor, speed) {
        int x = get<0>(coor);
        int y = get<1>(coor);
        adj_mat[x][y].cell = this;
        adj_mat[x][y].is_occupied = true;
        adj_mat[x][y].is_immune = true;
    };
    void next_iteration(int index) override;

};

