#ifndef CLASSES_H
#define CLASSES_H

#include <vector>
#include <tuple>

class Cell {
public:
    std::tuple<int, int> location;
    int speed;
    Cell(const std::tuple<int, int>& coor, int speed);
    std::vector<std::tuple<int, int>> get_neighbours(int radius);
    virtual void next_iteration(int index)=0;
};

class HealthyCell : public Cell {
public:
    HealthyCell(const std::tuple<int, int>& coor, int speed);
    void next_iteration(int index) override;

};

class SickCell : public Cell{
    int generation = 1;
public:
    SickCell(const std::tuple<int, int>& coor, int speed);
    SickCell(const std::tuple<int, int>& coor, int speed, int gen);
    void next_iteration(int index) override;
};

class ImmuneCell : public Cell {
public:
    ImmuneCell(const std::tuple<int, int>& coor, int speed);
    void next_iteration(int index) override;

};

struct matrixData {
    Cell* cell = nullptr;
    bool is_occupied = false;
    bool is_healthy = false;
    bool is_sick = false;
    bool is_immune = false;
};

#endif // CLASSES_H