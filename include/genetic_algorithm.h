#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include <vector>
#include "utils.h"

enum class StartingType
{
    ClarkeWright,
    NearestNeighbours,
    Random,
    Mixed
};

std::vector<std::vector<std::vector<int>>>
genetic_solver(
    const Matrix &distMatrix,
    const size_t maxPackages,
    const size_t populationSize,
    const size_t maxGenerations,
    StartingType startingType = StartingType::ClarkeWright);

#endif