#ifndef GENETIC_ALGO_UTILS_H
#define GENETIC_ALGO_UTILS_H

#include <vector>
#include <ostream>
struct Matrix;

// Struct representing a set of routes with a fitness level (total distance)
struct Individual
{
    std::vector<std::vector<int>> routes;
    double total_distance;

    Individual(const std::vector<std::vector<int>> &r = {}, double d = 0.0)
        : routes(r), total_distance(d) {}
};
std::ostream &operator<<(std::ostream &os, const Individual &individual);

std::vector<std::vector<int>> getRandomRoutes(const size_t distMatrixSize, const size_t maxPackages);
std::vector<Individual> getRandomPopulation(const Matrix &distMatrix, const size_t populationSize, const size_t maxPackages);
double routeDistance(const std::vector<int> &route, const Matrix &distMatrix);
double routeDistancePerLocation(const std::vector<int> &route, const Matrix &distMatrix);
double distanceOfRoutes(const std::vector<std::vector<int>> &routes, const Matrix &distMatrix);
std::vector<Individual> selectParents(const std::vector<Individual> &population, const size_t numOfParentCandidates, const size_t numOfParents);
void updateDistance(Individual &child, const Matrix &distMatrix);
Individual bestFromPopulation(const std::vector<Individual> &population);
Individual createNearestNeighbourIndividual(const Matrix &distMatrix, const size_t maxPackages);
Individual createCalrkeWrightIndividual(const Matrix &distMatrix, const size_t maxPackages);

#endif