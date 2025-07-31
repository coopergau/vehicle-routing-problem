#ifndef GENETIC_ALGO_UTILS_H
#define GENETIC_ALGO_UTILS_H

#include <vector>
#include <ostream>

// Struct representing on set of routes with its fitness level (total distance)
struct Individual
{
    std::vector<std::vector<int>> routes;
    double total_distance;

    Individual(const std::vector<std::vector<int>> &r, double d)
        : routes(r), total_distance(d) {}
};
std::ostream &operator<<(std::ostream &os, const Individual &individual);

std::vector<std::vector<int>> getRandomRoutes(size_t distMatrixSize, size_t maxPackages);
std::vector<Individual> getRandomPopulation(const std::vector<std::vector<double>> distMatrix, size_t populationSize, size_t maxPackages);
double routeDistancePerLocation(const std::vector<int> &route, const std::vector<std::vector<double>> &distMatrix);
double distanceOfRoutes(const std::vector<std::vector<int>> &routes, const std::vector<std::vector<double>> &distMatrix);
std::vector<Individual> selectParents(const std::vector<Individual> &population, size_t numOfParentCandidates, size_t numOfParents);
void updateDistance(Individual &child, const std::vector<std::vector<double>> &distMatrix);
Individual bestFromPopulation(const std::vector<Individual> &population);

#endif