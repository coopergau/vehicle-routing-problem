#ifndef CLARKE_WRIGHT_H
#define CLARKE_WRIGHT_H

#include <vector>
#include "utils.h"

std::pair<std::vector<std::vector<int>>, std::vector<std::vector<std::vector<int>>>>
clarkeWrightSolver(const Matrix &distMatrix, const size_t maxPackages);

std::pair<std::vector<std::vector<int>>, std::vector<std::vector<std::vector<int>>>>
processSavings(const std::vector<std::tuple<int, int, double>> &savings, size_t numCustomers, size_t maxPackages);

#endif