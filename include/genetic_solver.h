#ifndef GENETIC_SOLVER_H
#define GENETIC_SOLVER_H

#include <vector>

std::vector<std::vector<std::vector<int>>> genetic_solver(
    std::vector<std::vector<int>> startingRoutes,
    std::vector<std::vector<double>> &distMatrix,
    int maxPackages);

#endif