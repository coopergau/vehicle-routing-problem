#include "genetic_algo_utils.h"
#include "create_child.h"
#include "utils.h"
#include "clarke_wright.h"
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
    // vector populateed from 1 to size-1
    std::vector<int> locations(distMatrixSize - 1);
    std::iota(locations.begin(), locations.end(), 1);

    // randomly shuffle the vector
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(locations.begin(), locations.end(), gen);

    // split into routes of size 2 less than maxPackages to allow room for mutatuion
    std::vector<std::vector<int>> routes;
    for (size_t i = 0; i < locations.size(); i += maxPackages - 2)
    {
        size_t end = std::min(locations.size(), i + maxPackages - 2);
        std::vector<int> route(locations.begin() + i, locations.begin() + end);
        route.insert(route.begin(), 0);
        route.push_back(0);
        routes.push_back(route);
    }

    return routes;
}

std::vector<Individual> getRandomPopulation(const Matrix &distMatrix, size_t populationSize, size_t maxPackages)
{
    std::vector<Individual> population;
    population.reserve(populationSize);
    for (size_t i = 0; i < populationSize; ++i)
    {
        std::vector<std::vector<int>> routes = getRandomRoutes(distMatrix.rows.size(), maxPackages);
        double totalDistance = distanceOfRoutes(routes, distMatrix);
        population.emplace_back(routes, totalDistance);
    }
    return population;
}

double routeDistance(const std::vector<int> &route, const Matrix &distMatrix)
{
    double distance = 0.0;
    for (size_t i = 0; i < route.size() - 1; ++i)
    {
        distance += distMatrix.rows[route[i]][route[i + 1]];
    }
    return distance;
}

double routeDistancePerLocation(const std::vector<int> &route, const Matrix &distMatrix)
{
    double distance = routeDistance(route, distMatrix);
    return distance / route.size();
}

double distanceOfRoutes(const std::vector<std::vector<int>> &routes, const Matrix &distMatrix)
{
    double total_distance;
    for (const auto &route : routes)
    {
        total_distance += routeDistance(route, distMatrix);
    }
    return total_distance;
}

std::vector<Individual> selectParents(const std::vector<Individual> &population, size_t numOfParentCandidates, size_t numOfParents)
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

void updateDistance(Individual &child, const Matrix &distMatrix)
{
    child.total_distance = distanceOfRoutes(child.routes, distMatrix);
}

Individual bestFromPopulation(const std::vector<Individual> &population)
{
    Individual bestIndiv = population[0];
    for (size_t i = 1; i < population.size(); i++)
    {
        if (population[i].total_distance < bestIndiv.total_distance)
        {
            bestIndiv = population[i];
        }
    }

    return bestIndiv;
}

Individual createNearestNeighbourIndividual(const Matrix &distMatrix, size_t maxPackages)
{
    std::vector<int> unUsedLocations(distMatrix.rows.size() - 1);
    std::iota(unUsedLocations.begin(), unUsedLocations.end(), 1);
    std::vector<std::vector<int>> routes = {};

    while (!unUsedLocations.empty())
    {
        int lastLoc;
        bool newRoute = routes.size() == 0 || routes.back().size() == maxPackages / 2;
        if (newRoute)
        {
            lastLoc = 0;
        }
        else
        {
            lastLoc = routes.back().back();
        }

        int minLoc = 0;
        double minDist = std::numeric_limits<double>::infinity();
        for (const auto loc : unUsedLocations)
        {
            if (distMatrix.rows[lastLoc][loc] < minDist)
            {
                minDist = distMatrix.rows[lastLoc][loc];
                minLoc = loc;
            }
        }

        if (newRoute)
        {
            routes.push_back({minLoc});
        }
        else
        {
            routes.back().push_back(minLoc);
        }
        auto minLocIdx = std::find(unUsedLocations.begin(), unUsedLocations.end(), minLoc);
        unUsedLocations.erase(minLocIdx);
    }

    for (auto &route : routes)
    {
        route.insert(route.begin(), 0);
        route.push_back(0);
    }

    double routesDistance = distanceOfRoutes(routes, distMatrix);
    return Individual(routes, routesDistance);
}

Individual createCalrkeWrightIndividual(const Matrix &distMatrix, size_t maxPackages)
{
    auto [routes, routesProgress] = clarkeWrightSolver(distMatrix, maxPackages);
    double totalDistance = distanceOfRoutes(routes, distMatrix);
    return Individual(routes, totalDistance);
}