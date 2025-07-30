#ifndef GENETIC_ALGO_UTILS_H
#define GENETIC_ALGO_UTILS_H

#include <vector>

std::vector<std::vector<int>> getRandomRoutes(const size_t distMatrixSize, const size_t maxPackages);
std::vector<std::vector<std::vector<int>>> getRandomPopulation(const size_t populationSize, const size_t distMatrixSize, const size_t maxPackages);
double routeDistancePerLocation(const std::vector<int> &route, const std::vector<std::vector<double>> &distMatrix);
double distanceOfRoutes(const std::vector<std::vector<int>> &routes, const std::vector<std::vector<double>> &distMatrix);

#endif