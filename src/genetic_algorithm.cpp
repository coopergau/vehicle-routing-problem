#include "genetic_algorithm.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>

#include <iostream> // only for std::cout

std::vector<std::vector<int>> genetic_solver(
    std::vector<std::vector<double>> &distMatrix,
    int maxPackages)
{
    // Start with a set of randomly generated routes
    std::vector<int> locations(distMatrix.size() - 1);
    std::iota(locations.begin(), locations.end(), 1);

    // rng
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(locations.begin(), locations.end(), gen);

    // Split into routes
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
