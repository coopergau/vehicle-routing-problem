#include "genetic_algorithm.h"
#include "genetic_algo_utils.h"
#include "create_child.h"
#include "utils.h"
#include "utils.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <omp.h>

#include <iostream> // only for std::cout

#include "clarke_wright.h" // For trying to use the clarke-wright routes for the initial population

/* Genetic Algorithm Steps:
1. Start with some initial population of sets of routes
2. Evaluate the fitness of each set of routes (total distance)
3. Select the parents or the next generation via tournament style: For each parent randomly choose two possible candidates and select the one with the better fitness.
4-7 are performed for each two parents right after they are selected
    4. Copy the n best routes form the first parent to the child.
    5. Fill in the rest of the locations based on the second parent.
    6. Swap Mutation: With some small probability, randomly swap two locations in different routes.
    7. Memetic Algorithm: Perform a 2-opt search in each route (check how this affects performance).
8. Repeat Steps 2-7 until the max generations is hit.
*/
std::vector<std::vector<std::vector<int>>> genetic_solver(
    const Matrix &distMatrix,
    const size_t maxPackages,
    const size_t populationSize,
    const size_t maxGenerations,
    const float mutationProb,
    const StartingType startingType)
{
    if (mutationProb < 0.0 || mutationProb > 1.0)
    {
        throw std::invalid_argument("mutationProb must be between 0 and 1, got: " + std::to_string(mutationProb));
    }
    size_t numOfParentCandidates = 3;
    size_t numOfParents = 2; // The createChild function assumes 2 parents

    // Create 1st generation
    std::vector<Individual> population;
    population.reserve(populationSize);

    switch (startingType)
    {
    case StartingType::ClarkeWright:
    {
        Individual clarkeWrightIndividual = createCalrkeWrightIndividual(distMatrix, maxPackages);

        for (size_t i = 0; i < populationSize; ++i)
        {
            population.push_back(clarkeWrightIndividual);
        }
        break;
    }
    case StartingType::NearestNeighbours:
    {
        Individual nearestNeighbourIndividual = createNearestNeighbourIndividual(distMatrix, maxPackages);
        for (size_t i = 0; i < populationSize; ++i)
        {
            population.push_back(nearestNeighbourIndividual);
        }
        break;
    }
    case StartingType::Random:
    {
        population = getRandomPopulation(distMatrix, populationSize, maxPackages);
    }
    case StartingType::Mixed:
    {
        Individual clarkeWrightIndividual = createCalrkeWrightIndividual(distMatrix, maxPackages);
        Individual nearestNeighbourIndividual = createNearestNeighbourIndividual(distMatrix, maxPackages);

        population = getRandomPopulation(distMatrix, populationSize / 3, maxPackages);

        size_t remaining = populationSize - population.size();
        for (size_t i = 0; i < remaining; i += 2)
        {
            population.push_back(clarkeWrightIndividual);
            if (i + 1 < remaining)
            {
                population.push_back(nearestNeighbourIndividual);
            }
        }
        break;
    }
    default:
        throw std::invalid_argument("Starting type must be ClarkeWright, NearestNeighbours, Random, or Mixed");
    }

    Individual bestIndividual = bestFromPopulation(population);
    std::vector<std::vector<std::vector<int>>> bestRoutesProgress = {bestIndividual.routes};

    // Create the next generations
    for (size_t generation = 0; generation < maxGenerations; ++generation)
    {
        std::vector<Individual> newPopulation(populationSize);
#pragma omp parallel for
        for (size_t family = 0; family < populationSize; ++family)
        {
            std::vector<Individual> parents = selectParents(population, numOfParentCandidates, numOfParents);
            Individual child = createChild(parents, maxPackages, mutationProb, distMatrix);
            newPopulation[family] = child;
        }
        if (generation % 100 == 0)
        {
            std::cout << generation << std::endl;
        }
        population = newPopulation;

        Individual bestFromCurrentGen = bestFromPopulation(population);
        if (bestFromCurrentGen.total_distance < bestIndividual.total_distance)
        {
            bestIndividual = bestFromCurrentGen;
            bestRoutesProgress.push_back(bestIndividual.routes);
            std::cout << "new best " << bestIndividual.total_distance << std::endl;
        }
    }

    return bestRoutesProgress;
}
