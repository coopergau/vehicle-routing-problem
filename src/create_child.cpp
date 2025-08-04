#include "create_child.h"
#include "genetic_algo_utils.h"
#include "utils.h"
#include <vector>
#include <set>

#include <numeric>
#include <algorithm>
#include <random>
#include <iostream>
#include <unordered_set>

Individual createChild(const std::vector<Individual> &parents, size_t routesFromParentA, size_t maxPackages, const Matrix &distMatrix)
{
    // For testing without the crossover the child will just be the fittest parent
    Individual child;
    Individual parentA;
    Individual parentB;
    if (parents[0].total_distance < parents[1].total_distance)
    {
        parentA = parents[0];
        parentB = parents[1];
    }
    else
    {
        parentB = parents[0];
        parentA = parents[1];
    }
    float mutationProb = 0.5;

    child = routeCrossover(parentA, parentB, maxPackages, distMatrix);
    mutation(child, mutationProb, maxPackages);
    twoOptSwap(child, distMatrix);
    updateDistance(child, distMatrix);
    return child;
}

Individual routeCrossover(const Individual &parentA, const Individual &parentB, size_t maxPackages, const Matrix &distMatrix)
{
    std::unordered_set<int> used;
    std::vector<std::vector<int>> childRoutes;

    // Copy half of the routes from parentA
    for (size_t i = 0; i < parentA.routes.size() / 2; ++i)
    {
        childRoutes.push_back(parentA.routes[i]);
        for (int node : parentA.routes[i])
        {
            used.insert(node);
        }
    }

    // Fill remaining from parentB
    for (const auto &route : parentB.routes)
    {
        std::vector<int> newRoute;
        for (int node : route)
        {
            if (used.count(node) == 0)
            {
                newRoute.push_back(node);
                used.insert(node);
            }
        }
        if (!newRoute.empty())
        {
            newRoute.push_back(0);
            newRoute.insert(newRoute.begin(), 0);
            childRoutes.push_back(newRoute);
        }
    }

    // Check if combining any routes saves on distance
    for (size_t i = 0; i < childRoutes.size() - 1; ++i)
    {
        for (size_t j = i + 1; j < childRoutes.size(); ++j)
        {
            if (childRoutes[i].size() + childRoutes[j].size() <= maxPackages + 2)
            {
                double distA = routeDistancePerLocation(childRoutes[i], distMatrix) * childRoutes[i].size();
                double distB = routeDistancePerLocation(childRoutes[j], distMatrix) * childRoutes[j].size();
                double sumOfSeperateDistances = distA + distB;

                std::vector<int> combinedRoute = {0};
                combinedRoute.insert(combinedRoute.end(), childRoutes[i].begin() + 1, childRoutes[i].end() - 1);
                combinedRoute.insert(combinedRoute.end(), childRoutes[j].begin() + 1, childRoutes[j].end() - 1);
                combinedRoute.push_back(0);
                double combinedDistance = routeDistancePerLocation(combinedRoute, distMatrix);

                if (combinedDistance < sumOfSeperateDistances)
                {
                    childRoutes.erase(childRoutes.begin() + j);
                    childRoutes.erase(childRoutes.begin() + i);
                    childRoutes.push_back(combinedRoute);

                    i = -1;
                    break;
                }
            }
        }
    }

    return Individual(childRoutes);
}

void mutation(Individual &child, float mutationProbability, size_t maxPackages)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    float randomNum = dis(gen);

    if (randomNum <= mutationProbability)
    {
        moveRandomElement(child, maxPackages);
    }
}

void moveRandomElement(Individual &child, size_t maxPackages)
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
}

void twoOptSwap(Individual &child, const Matrix &distMatrix)
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
        double shortestRouteLength = routeDistancePerLocation(route, distMatrix);
        for (size_t i = 1; i < route.size() - 1; i++)
        {
            for (size_t j = i + 2; j < route.size() - 1; j++)
            {
                std::vector<int> newRoute = route;
                std::reverse(newRoute.begin() + i + 1, newRoute.begin() + j + 1);
                double newRouteLength = routeDistancePerLocation(newRoute, distMatrix);
                if (newRouteLength < shortestRouteLength)
                {
                    shortestRoute = newRoute;
                    shortestRouteLength = newRouteLength;
                    i = 1;
                    j = i + 2;
                }
            }
        }
        newRoutes.push_back(shortestRoute);
    }
    child.routes = newRoutes;
}