#ifndef CLARKE_WRIGHT_H
#define CLARKE_WRIGHT_H

#include <vector>

std::pair<std::vector<std::vector<int>>, std::vector<std::vector<std::vector<int>>>>
clarkeWrightSolver(const std::vector<std::vector<double>> &distMatrix, const size_t maxPackages);

std::pair<std::vector<std::vector<int>>, std::vector<std::vector<std::vector<int>>>>
processSavings(const std::vector<std::tuple<int, int, double>> &savings, size_t numCustomers, size_t maxPackages);

#endif