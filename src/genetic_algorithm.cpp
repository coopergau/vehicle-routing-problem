#include "genetic_algorithm.h"
#include "genetic_algo_utils.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>

#include <iostream> // only for std::cout

/* Genetic Algorithm Steps:
1. Start with some initial population of sets of routes
2. Evaluate the fitness of each set of routes
3. Select the parents or the next generation via tournament style: For each parent randomly choose two possible candidates and select the one with the better fitness.
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
    // 1.
    std::vector<std::vector<std::vector<int>>> population = getRandomPopulation(populationSize, distMatrix.size(), maxPackages);

    for (const auto &routes : population)
    {
        for (const auto &route : routes)
        {
            for (const auto loc : route)
            {
                std::cout << loc << " ";
            }
            std::cout << std::endl;
            double distance = routeDistancePerLocation(route, distMatrix);
            std::cout << distance;
            std::cout << std::endl;
        }
        double totalDistance = distanceOfRoutes(routes, distMatrix);
        std::cout << totalDistance;
        std::cout << std::endl;
    }

    return {};
}
