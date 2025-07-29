#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include <vector>

std::vector<std::vector<int>> genetic_solver(
    const std::vector<std::vector<double>> &distMatrix,
    const int maxPackages,
    const size_t populationSize,
    const int maxGenerations);
std::vector<std::vector<int>> getRandomRoutes(const size_t distMatrixSize, const int maxPackages);
std::vector<std::vector<std::vector<int>>> getRandomPopulation(const size_t populationSize, const size_t distMatrixSize, const int maxPackages);
double routeDistancePerLocation(const std::vector<int> &route, const std::vector<std::vector<double>> &distMatrix);
double distanceOfRoutes(const std::vector<std::vector<int>> &routes, const std::vector<std::vector<double>> &distMatrix);

#endif