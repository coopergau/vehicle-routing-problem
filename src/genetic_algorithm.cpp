#include "genetic_algorithm.h"
#include "genetic_algo_utils.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>

#include <iostream> // only for std::cout

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
std::vector<std::vector<int>> genetic_solver(
    const std::vector<std::vector<double>> &distMatrix,
    const size_t maxPackages,
    const size_t populationSize,
    const size_t maxGenerations)
{
    size_t numOfParentCandidates = 3;
    size_t numOfParents = 2; // The createChild function assumes 2 parents
    size_t routesFromParentOne = 6;

    // 1. & 2.
    std::vector<Individual> population = getRandomPopulation(distMatrix, populationSize, maxPackages);

    // 3. & 4.
    for (size_t generation = 0; generation < 2; generation++)
    {
        std::vector<Individual> newPopulation;
        newPopulation.reserve(populationSize);
        for (size_t family = 0; family < populationSize; family++)
        {
            std::vector<Individual> parents = selectParents(population, numOfParentCandidates, numOfParents);
            Individual child = createChild(parents, routesFromParentOne, distMatrix);
            newPopulation.push_back(child);
            std::cout << child;
        }
        population = newPopulation;
    }

    return {};
}
