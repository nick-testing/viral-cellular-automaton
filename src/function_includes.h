#ifndef FUNCTION_INCLUDES_H
#define FUNCTION_INCLUDES_H

#include "include.h"
#include "classes.cpp"


// Global variable counters
int N;                 // Total number of occupied cells
double D = 0.002;      // Fraction of starting sick cells
double R = 0.01;       // Fraction of fast moving cells
int X = 2;             // Generations until sick cells become immune
int P1 = 60;           // Probability (percentage) of people getting sick when not taking care
int P2 = 5;            // Probability (percentage) of people getting sick when taking care
int T;                 // P1 to P2 threshold - 0.1 * N Default

int temp_sick_counter;    // Will store mid-iteration sick number of each iteration.
int sick_counter = 0;
int healthy_counter = 0;
int immune_counter = 0;

std::random_device rd;

vector<Cell*> cell_array; // vector that stores pointers to all occupied cells.

// Output functions
void clear();
void print_matrix(matrixData adj_mat[200][200]);
void get_params();

#endif