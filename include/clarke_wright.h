#ifndef CLARKE_WRIGHT_H
#define CLARKE_WRIGHT_H

#include <vector>

std::vector<std::vector<int>> clarkeWrightSolver(std::vector<std::vector<double>> &distMatrix, int maxPackages);
std::vector<std::vector<int>> processSavings(std::vector<std::tuple<int, int, double>> &savings, int numCustomers, int maxPackages);

#endif