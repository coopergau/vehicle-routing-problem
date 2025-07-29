#include "genetic_algorithm.h"
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
    const int maxPackages,
    const size_t populationSize,
    const int maxGenerations)
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

std::vector<std::vector<int>> getRandomRoutes(const size_t distMatrixSize, const int maxPackages)
{
    // vector fom 1 to size-1
    std::vector<int> locations(distMatrixSize - 1);
    std::iota(locations.begin(), locations.end(), 1);

    // rng
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(locations.begin(), locations.end(), gen);

    // split into routes
    std::vector<std::vector<int>> routes;
    for (size_t i = 0; i < locations.size(); i += maxPackages)
    {
        size_t end = std::min(locations.size(), i + maxPackages);
        std::vector<int> route(locations.begin() + i, locations.begin() + end);
        route.insert(route.begin(), 0);
        route.push_back(0);
        routes.push_back(route);
    }

    for (const auto &route : routes)
    {
        for (const auto loc : route)
        {
            std::cout << loc << " ";
        }
        std::cout << std::endl;
    }

    return routes;
}

std::vector<std::vector<std::vector<int>>> getRandomPopulation(const size_t populationSize, const size_t distMatrixSize, const int maxPackages)
{
    std::vector<std::vector<std::vector<int>>> population(populationSize);
    for (size_t i = 0; i < populationSize; ++i)
    {
        population.push_back(getRandomRoutes(distMatrixSize, maxPackages));
    }
    return population;
}

// Route fitness = route distance / nummber of locations in route
double routeDistancePerLocation(const std::vector<int> &route, const std::vector<std::vector<double>> &distMatrix)
{
    double distance;
    for (size_t i = 0; i < route.size() - 1; ++i)
    {
        distance += distMatrix[route[i]][route[i + 1]];
    }
    return distance / route.size();
}

double distanceOfRoutes(const std::vector<std::vector<int>> &routes, const std::vector<std::vector<double>> &distMatrix)
{
    double total_distance;
    for (const auto &route : routes)
    {
        total_distance += routeDistancePerLocation(route, distMatrix) * route.size();
    }
    return total_distance;
}