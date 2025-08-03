#include "genetic_algorithm.h"
#include "genetic_algo_utils.h"
#include "create_child.h"
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
    const std::vector<std::vector<double>> &distMatrix,
    const size_t maxPackages,
    const size_t populationSize,
    const size_t maxGenerations)
{
    size_t numOfParentCandidates = 3;
    size_t numOfParents = 2; // The createChild function assumes 2 parents
    size_t routesFromParentOne = 6;

    // 1. & 2.
    // Starting random
    // std::vector<Individual> population = getRandomPopulation(distMatrix, populationSize, maxPackages);

    // Population clarkewright
    /*std::vector<Individual> population;
    population.reserve(populationSize);
    for (size_t i = 0; i < populationSize / 2; i++)
    {
        auto [routesByIndex, routesProgress] = clarkeWrightSolver(distMatrix, maxPackages);
        double totalDistance = distanceOfRoutes(routesByIndex, distMatrix);
        std::cout << "Initial distance: " << totalDistance << std::endl;
        population.emplace_back(routesByIndex, totalDistance);
    }

    std::vector<Individual> randomPopulation = getRandomPopulation(distMatrix, populationSize / 2, maxPackages);
    population.insert(population.end(), randomPopulation.begin(), randomPopulation.end());*/

    // Starting with nearest neighbour
    Individual nearestNeighbourIndiv = nearestNeighbourRoutes(distMatrix, maxPackages);
    std::cout << "Initial distance: " << nearestNeighbourIndiv.total_distance << std::endl;
    std::vector<Individual> population(populationSize, nearestNeighbourIndiv);

    // population.insert(population.end(), NNpopulation.begin(), NNpopulation.end());

    Individual bestIndividual = bestFromPopulation(population);
    std::vector<std::vector<std::vector<int>>> bestRoutesProgress = {bestIndividual.routes};

    // 3. & 4.
    for (size_t generation = 0; generation < maxGenerations; generation++)
    {
        std::vector<Individual> newPopulation(populationSize);
#pragma omp parallel for
        for (int family = 0; family < static_cast<int>(populationSize); ++family)
        {
            std::vector<Individual> parents = selectParents(population, numOfParentCandidates, numOfParents);
            Individual child = createChild(parents, routesFromParentOne, maxPackages, distMatrix);
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
