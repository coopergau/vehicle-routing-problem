#include "genetic_algo_utils.h"
#include "create_child.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <iostream>

std::ostream &operator<<(std::ostream &os, const Individual &individual)
{
    for (size_t i = 0; i < individual.routes.size(); ++i)
    {
        // continue;
        os << "Route " << i << ": ";
        for (int loc : individual.routes[i])
        {
            os << loc << " ";
        }
        os << std::endl;
    }
    os << "Total Distance: " << individual.total_distance << std::endl;
    return os;
}

std::vector<std::vector<int>> getRandomRoutes(size_t distMatrixSize, size_t maxPackages)
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

    return routes;
}

std::vector<Individual> getRandomPopulation(const std::vector<std::vector<double>> distMatrix, size_t populationSize, size_t maxPackages)
{
    std::vector<Individual> population;
    population.reserve(populationSize);
    for (size_t i = 0; i < populationSize; ++i)
    {
        std::vector<std::vector<int>> routes = getRandomRoutes(distMatrix.size(), maxPackages);
        double totalDistance = distanceOfRoutes(routes, distMatrix);
        population.emplace_back(routes, totalDistance);
    }
    return population;
}

// Route fitness = route distance / nummber of locations in route
double routeDistancePerLocation(const std::vector<int> &route, const std::vector<std::vector<double>> &distMatrix)
{
    double distance = 0.0;
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

std::vector<Individual> selectParents(const std::vector<Individual> population, size_t numOfParentCandidates, size_t numOfParents)
{
    // Randomly select numOfParentCandidates individuals and choose the best one as a parent.
    // Repeat NumOfParents times.

    std::vector<Individual> parents;
    for (size_t i = 0; i < numOfParents; ++i)
    {
        std::vector<Individual> parentCandidates;
        for (size_t j = 0; j < numOfParentCandidates; ++j)
        {
            int idx = rand() % population.size();
            parentCandidates.push_back(population[idx]);
        }
        Individual bestCandidate = *std::min_element(
            parentCandidates.begin(),
            parentCandidates.end(),
            [](const Individual &a, const Individual &b)
            {
                return a.total_distance < b.total_distance;
            });
        parents.push_back(bestCandidate);
    }

    return parents;
}

void updateDistance(Individual &child, const std::vector<std::vector<double>> &distMatrix)
{
    child.total_distance = distanceOfRoutes(child.routes, distMatrix);
}