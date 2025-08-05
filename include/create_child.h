#ifndef CREATE_CHILD_H
#define CREATE_CHILD_H

#include "genetic_algo_utils.h"
#include "utils.h"
#include <vector>

Individual createChild(const std::vector<Individual> &parents, const size_t maxPackages, const float mutationProb, const Matrix &distMatrix);
Individual routeCrossover(const Individual &parentA, const Individual &parentB, const size_t maxPackages, const Matrix &distMatrix);
void mutation(Individual &child, const float mutationProbability, const size_t maxPackages);
void moveRandomElement(Individual &child, const size_t maxPackages);
void twoOptSwap(Individual &child, const Matrix &distMatrix);

#endif