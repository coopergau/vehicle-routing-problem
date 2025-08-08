#include "genetic_algorithm.h"
#include "genetic_algo_utils.h"
#include "create_child.h"
#include "utils.h"
#include "clarke_wright.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <omp.h>
#include <iostream>

/* Genetic Algorithm Steps:
1. Start with some initial population of sets of routes, dictated by startingType.
2. Evaluate the fitness of each set of routes (total distance)
3. Select the parents or the next generation via tournament style: For each parent randomly choose three possible candidates and select the one with the better fitness.
4-6 are performed for each two parents right after they are selected, in the createChild function.
    4. Route Crossover: Copy half of the fittest parent's routes to intialize the child routes. Fill in the rest of the locations based on the second parent.
       Check if combining any routes saves on distance.
    5. Mutation: With some probability, randomly move one location to a different route.
    6. Memetic Algorithm: Perform a 2-opt search in each route.
7. Repeat Steps 2-6 until the maximum number of generations is hit.
*/
std::vector<std::vector<std::vector<int>>> geneticSolver(
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
    if (maxPackages < 2)
    {
        throw std::invalid_argument("maxPackages must be greater than 2, got: " + std::to_string(maxPackages));
    }
    if (distMatrix.rows.size() == 0)
    {
        throw std::invalid_argument("distance matrix was empty");
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
        break;
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
        population = newPopulation;

        Individual bestFromCurrentGen = bestFromPopulation(population);
        if (bestFromCurrentGen.total_distance < bestIndividual.total_distance)
        {
            bestIndividual = bestFromCurrentGen;
            bestRoutesProgress.push_back(bestIndividual.routes);
        }
    }

    return bestRoutesProgress;
}
