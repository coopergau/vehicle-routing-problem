#include "clarke_wright.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include <utility>

/* Clarke-Wright algorithm
    - Returns a final version of the routes and the vector of the routes at each step of the algorithm.
    - Does not alow for specifying the number of routes (vehicles).
    - Starts with the one depot at the first row and col of matrix.
*/
std::pair<std::vector<std::vector<int>>, std::vector<std::vector<std::vector<int>>>>
clarkeWrightSolver(const std::vector<std::vector<double>> &distMatrix, const size_t maxPackages)
{
    // 1. Create savings list and order by descending savings amounts
    std::vector<std::tuple<int, int, double>> savings;

    size_t numLocations = distMatrix.size();
    for (int i = 1; i < numLocations; i++)
    {
        for (int j = i + 1; j < numLocations; j++)
        {
            double saving = distMatrix[0][i] + distMatrix[0][j] - distMatrix[i][j];
            savings.emplace_back(i, j, saving);
        }
    }

    std::sort(savings.begin(), savings.end(), [](const auto &a, const auto &b)
              { return std::get<2>(a) > std::get<2>(b); });

    /* 2. Iterate through sorted savings list and do one of three things as long it does not create a route with more locations than maxPackages:
            - If both points i and j have not been included in a route create a new route by connecting them
            - If only one of i or j has been included in a route and it is not in the interior of the route, the link i-j will be added to the route
            - If both i and j have been included in a route and they are both not interior points, connect their routes
        3. Any points not included in a route create their own route that consists of only that point.
        4. Add the depot to the beginning and ending of each route.
        Steps 2-4 are done in ProcessSavings
    */
    int numCustomers = numLocations - 1; // minus the one depot
    auto [routes, routesProgress] = processSavings(savings, numCustomers, maxPackages);

    return {routes, routesProgress};
}

// Seperated the function for easier testing
std::pair<std::vector<std::vector<int>>, std::vector<std::vector<std::vector<int>>>>
processSavings(const std::vector<std::tuple<int, int, double>> &savings, const size_t numCustomers, const size_t maxPackages)
{
    /* 2. Iterate through sorted savings list and do one of three things:
            - If both points i and j have not been included in a route create a new route by connecting them
            - If only one of i or j has been included in a route and it is not in the interior of the route, the link i-j will be added to the route
            - If both i and j have been included in a route and they are both not interior points, connect their routes
    */
    std::vector<std::vector<int>> routes;
    std::vector<std::vector<std::vector<int>>> routesProgress;
    std::vector<bool> isEdgePoint(numCustomers, false);
    std::unordered_map<int, int> pointToRoute; // Maps a point index to its route index

    for (const auto &s : savings)
    {
        int i = std::get<0>(s);
        int j = std::get<1>(s);

        // Neither in route
        if (pointToRoute.find(i) == pointToRoute.end() && pointToRoute.find(j) == pointToRoute.end())
        {
            std::vector<int> newRoute = {i, j};
            int routeIndex = routes.size();
            routes.push_back(newRoute);

            pointToRoute[i] = routeIndex;
            pointToRoute[j] = routeIndex;

            isEdgePoint[i] = true;
            isEdgePoint[j] = true;

            routesProgress.push_back(routes);
        }

        // One (at index i) is an edge point and other point is not in a route and the route is less than the max length
        else if (isEdgePoint[i] && pointToRoute.find(j) == pointToRoute.end() && routes[pointToRoute[i]].size() != maxPackages)
        {
            std::vector<int> &route = routes[pointToRoute[i]];

            if (route.front() == i)
            {
                route.insert(route.begin(), j);
            }
            else if (route.back() == i)
            {
                route.push_back(j);
            }

            isEdgePoint[i] = false;
            isEdgePoint[j] = true;
            pointToRoute[j] = pointToRoute[i];

            routesProgress.push_back(routes);
        }

        // One (at index j) is an edge point and other point is not in a route and the route is less than the max length
        else if (isEdgePoint[j] && pointToRoute.find(i) == pointToRoute.end() && routes[pointToRoute[j]].size() != maxPackages)
        {
            std::vector<int> &route = routes[pointToRoute[j]];
            if (route.front() == j)
            {
                route.insert(route.begin(), i);
            }
            else if (route.back() == j)
            {
                route.push_back(i);
            }

            isEdgePoint[j] = false;
            isEdgePoint[i] = true;
            pointToRoute[i] = pointToRoute[j];

            routesProgress.push_back(routes);
        }

        // Both are end points and they are on different routes and combining them will not violate max route length
        else if (isEdgePoint[i] && isEdgePoint[j] &&
                 pointToRoute[i] != pointToRoute[j] &&
                 routes[pointToRoute[i]].size() + routes[pointToRoute[j]].size() <= maxPackages)
        {
            int routeIndexI = pointToRoute[i];
            int routeIndexJ = pointToRoute[j];

            // Make routeI the one with the smaller index
            if (routeIndexI > routeIndexJ)
            {
                std::swap(routeIndexI, routeIndexJ);
                std::swap(i, j);
            }

            std::vector<int> &routeI = routes[routeIndexI];
            std::vector<int> &routeJ = routes[routeIndexJ];

            // Combine vectors, reverse if i and j are both at the front or both at the back
            if (routeI.front() == i)
            {
                if (routeJ.front() == j)
                {
                    std::reverse(routeJ.begin(), routeJ.end());
                    routeI.insert(routeI.begin(), routeJ.begin(), routeJ.end());
                }
                else if (routeJ.back() == j)
                {
                    routeI.insert(routeI.begin(), routeJ.begin(), routeJ.end());
                }
            }
            else if (routeI.back() == i)
            {
                if (routeJ.front() == j)
                {
                    routeI.insert(routeI.end(), routeJ.begin(), routeJ.end());
                }
                else if (routeJ.back() == j)
                {
                    std::reverse(routeJ.begin(), routeJ.end());
                    routeI.insert(routeI.end(), routeJ.begin(), routeJ.end());
                }
            }

            isEdgePoint[i] = false;
            isEdgePoint[j] = false;

            // Update the pointToRoute indices
            for (int point : routeJ)
            {
                pointToRoute[point] = routeIndexI;
            }

            routes.erase(routes.begin() + routeIndexJ);
            if (routeIndexJ < routes.size())
            {
                for (int row = routeIndexJ; row < routes.size(); row++)
                {
                    for (int point : routes[row])
                    {
                        pointToRoute[point] = row;
                    }
                }
            }

            routesProgress.push_back(routes);
        }
    }

    // 3. Any points not included in a route create their own route that consists of only that point.
    for (int i = 1; i <= numCustomers; i++)
    {
        if (pointToRoute.find(i) == pointToRoute.end())
        {
            std::vector<int> newRoute = {i};
            routes.push_back(newRoute);
            routesProgress.push_back(routes);
        }
    }

    // 4. Add the depot to the beginning and ending of each route.
    for (auto &route : routes)
    {
        route.insert(route.begin(), 0);
        route.push_back(0);
    }

    return {routes, routesProgress};
}