#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include "utils.h"
#include <vector>

// Different starting types for geneticSolver. Mixed creates a population with one third coming from the other types.
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