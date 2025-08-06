#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include <vector>
#include "utils.h"

enum class StartingType
{
    ClarkeWright,
    NearestNeighbours,
    Random,
    Mixed,
    COUNT
};

std::vector<std::vector<std::vector<int>>>
geneticSolver(
    const Matrix &distMatrix,
    const size_t maxPackages,
    const size_t populationSize,
    const size_t maxGenerations,
    const float mutationProb,
    const StartingType startingType = StartingType::ClarkeWright);

#endif