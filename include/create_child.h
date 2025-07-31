#ifndef CREATE_CHILD_H
#define CREATE_CHILD_H

#include "genetic_algo_utils.h"
#include <vector>

Individual createChild(const std::vector<Individual> &parents, size_t routesFromParentA, size_t maxPackages, const std::vector<std::vector<double>> &distMatrix);
Individual routeCrossover(const std::vector<Individual> &parents, size_t routesFromParentA, const std::vector<std::vector<double>> &distMatrix);
void mutation(Individual &child, float mutationProbability, size_t maxPackages, const std::vector<std::vector<double>> &distMatrix);
void moveRandomElement(Individual &child, size_t maxPackages, const std::vector<std::vector<double>> &distMatrix);

#endif