#ifndef CREATE_CHILD_H
#define CREATE_CHILD_H

#include "genetic_algo_utils.h"
#include <vector>

Individual createChild(const std::vector<Individual> &parents, size_t routesFromParentA, const std::vector<std::vector<double>> &distMatrix);
Individual routeCrossover(const std::vector<Individual> &parents, size_t routesFromParentA, const std::vector<std::vector<double>> &distMatrix);
void mutation(Individual &child, float mutationProbability, const std::vector<std::vector<double>> &distMatrix);
void moveRandomElement(Individual &child);

#endif