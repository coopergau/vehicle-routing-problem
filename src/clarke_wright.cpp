#include "clarke_wright.h"
#include "point.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

// Does not alow for specifying the number of routes (vehicles).
// Starts with the one depot at the first row and col of matrix.
std::vector<std::vector<int>>
clarkeWrightSolver(std::vector<std::vector<double>> distMatrix)
{
    // 1. Create savings list and order by descending savings amounts
    std::vector<std::tuple<int, int, double>> savings;

    int numCustomers = distMatrix.size();
    for (int i = 1; i < numCustomers; i++)
    {
        for (int j = i + 1; j < numCustomers; j++)
        {
            double saving = distMatrix[0][i] + distMatrix[0][j] - distMatrix[i][j];
            savings.emplace_back(i, j, saving);
        }
    }

    std::sort(savings.begin(), savings.end(), [](const auto &a, const auto &b)
              { return std::get<2>(a) > std::get<2>(b); });

    /* 2. Iterate through sorted savings list and do one of three things:
            - If both points i and j have not been included in a route create a new route by connecting them
            - If only one of i or j has been included in a route and it is not in the interior of the route, the link i-j will be added to the route
            - If both i and j have been included in a route and they are both not interior points, connect their routes
    */
    std::vector<std::vector<Point>> routes;
    std::vector<bool> isEdgePoint(numCustomers, false);

    for (const auto &s : savings)
    {
        int i = std::get<0>(s);
        int j = std::get<1>(s);

        // Neither in route
        if (!isEdgePoint[i] && !isEdgePoint[j])
        {
            // Make new route
            isEdgePoint[i] = true;
            isEdgePoint[j] = true;
        }

        // One is an edge point
        else if (!isEdgePoint[i] && isEdgePoint[j] ||
                 isEdgePoint[i] && !isEdgePoint[j])
        {
            // Add the excluded one to route
        }

        // Both are end points
        else if (isEdgePoint[i] && isEdgePoint[j])
        {
            // Combine routes
        }
    }

    for (const auto &s : savings)
    {
        std::cout << "S(" << std::get<0>(s) << ", " << std::get<1>(s) << ") = " << std::get<2>(s) << "\n";
    }

    std::vector<std::vector<int>> placeholder;
    return placeholder;
}