#include "create_child.h"
#include "genetic_algo_utils.h"
#include <vector>
#include <set>

#include <numeric>
#include <algorithm>
#include <random>
#include <iostream>

Individual createChild(const std::vector<Individual> &parents, size_t routesFromParentA, size_t maxPackages, const std::vector<std::vector<double>> &distMatrix)
{
    // For testing without the crossover the child will jsut be parentA
    Individual child = parents[0];
    float mutationProb = 1;
    mutation(child, mutationProb, maxPackages, distMatrix);
    twoOptSwap(child, distMatrix);
    return child;
}

Individual routeCrossover(const std::vector<Individual> &parents, size_t routesFromParentA, const std::vector<std::vector<double>> &distMatrix)
{
    // Note this function assumes 2 parents
    // Populate the child with routesFromParentA
    // ------------------------------------------------------------make this just parentARoutes
    Individual parentA = parents[0];

    std::sort(parentA.routes.begin(), parentA.routes.end(),
              [&distMatrix](const std::vector<int> &a, const std::vector<int> &b)
              {
                  double distA = routeDistancePerLocation(a, distMatrix);
                  double distB = routeDistancePerLocation(b, distMatrix);
                  return distA < distB;
              });

    std::vector<std::vector<int>> childRoutes(parentA.routes.begin(), parentA.routes.begin() + routesFromParentA);
    double childRoutesDistance = distanceOfRoutes(childRoutes, distMatrix);
    Individual child(childRoutes, childRoutesDistance);

    // Fill in the rest with routes influenced by the order of parentB's routes
    Individual parentB = parents[1];
    std::set<int> childLocations;
    for (const auto &route : child.routes)
    {
        for (int location : route)
        {
            if (location != 0)
            {
                childLocations.insert(location);
            }
        }
    }

    for (auto &route : parentB.routes)
    {
        route.erase(
            std::remove_if(route.begin(), route.end(),
                           [&childLocations](int location)
                           { return childLocations.find(location) != childLocations.end(); }),
            route.end());
    }

    parentB.routes.erase(
        std::remove_if(parentB.routes.begin(), parentB.routes.end(),
                       [](const std::vector<int> &route)
                       { return route.size() <= 2; }),
        parentB.routes.end());

    child.routes.insert(child.routes.end(), parentB.routes.begin(), parentB.routes.end());
    child.total_distance = distanceOfRoutes(child.routes, distMatrix);

    return child;
}

void mutation(Individual &child, float mutationProbability, size_t maxPackages, const std::vector<std::vector<double>> &distMatrix)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    float randomNum = dis(gen);

    if (randomNum <= mutationProbability)
    {
        moveRandomElement(child, maxPackages, distMatrix);
    }
}

void moveRandomElement(Individual &child, size_t maxPackages, const std::vector<std::vector<double>> &distMatrix)
{
    // In case the new routes don't fit the constraints
    std::vector<std::vector<int>> originalRoutes = child.routes;

    // Randomly selects one location and moves it to a random new place in the routes.
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<int> routeDist(0, child.routes.size() - 1);
    int sourceRouteIdx = routeDist(gen);

    std::uniform_int_distribution<int> elemDist(1, child.routes[sourceRouteIdx].size() - 2);
    int sourceElementIdx = elemDist(gen);

    int element = child.routes[sourceRouteIdx][sourceElementIdx];
    child.routes[sourceRouteIdx].erase(child.routes[sourceRouteIdx].begin() + sourceElementIdx);
    if (child.routes[sourceRouteIdx].size() == 2)
    {
        child.routes.erase(child.routes.begin() + sourceRouteIdx);
    }

    int destRouteIdx = sourceRouteIdx;
    int destElementIdx = sourceElementIdx;

    while (destRouteIdx == sourceRouteIdx && destElementIdx == sourceElementIdx)
    {
        std::uniform_int_distribution<int> routeDist(0, child.routes.size());
        destRouteIdx = routeDist(gen);
        if (destRouteIdx == child.routes.size())
        {
            child.routes.push_back({0, element, 0});
            updateDistance(child, distMatrix);
            return;
        }
        std::uniform_int_distribution<int> elemDist(1, child.routes[destRouteIdx].size() - 2);
        destElementIdx = elemDist(gen);
    }

    if (child.routes[destRouteIdx].size() == maxPackages + 2)
    {
        child.routes = originalRoutes;
        return;
    }
    child.routes[destRouteIdx].insert(child.routes[destRouteIdx].begin() + destElementIdx, element);
    updateDistance(child, distMatrix);
}

void twoOptSwap(Individual &child, const std::vector<std::vector<double>> &distMatrix)
{
    std::vector<std::vector<int>> newRoutes;
    for (const auto &route : child.routes)
    {
        if (route.size() < 5)
        {
            newRoutes.push_back(route);
            continue;
        }
        std::vector<int> shortestRoute = route;
        double shortestRouteLength = routeDistancePerLocation(route, distMatrix) * route.size();
        for (size_t i = 1; i < route.size() - 1; i++)
        {
            for (size_t j = i + 2; j < route.size() - 1; j++)
            {
                std::vector<int> newRoute = route;
                std::reverse(newRoute.begin() + i + 1, newRoute.begin() + j + 1);
                double newRouteLength = routeDistancePerLocation(newRoute, distMatrix) * route.size();
                if (newRouteLength < shortestRouteLength)
                {
                    shortestRoute = newRoute;
                    shortestRouteLength = newRouteLength;
                }
            }
        }
        newRoutes.push_back(shortestRoute);
    }
    child.routes = newRoutes;
}