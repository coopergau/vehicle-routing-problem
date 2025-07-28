#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include <vector>

std::vector<std::vector<int>> genetic_solver(
    std::vector<std::vector<double>> &distMatrix,
    int maxPackages);

#endif