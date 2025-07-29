#include <iostream>
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <limits>
#include <cmath>

#include "utils.h"
#include "clarke_wright.h"
#include "genetic_algorithm.h"

int main()
{
    const int numCustomers = 100;
    const int numDepots = 1;
    const int maxPackages = 10;
    const size_t populationSize = 50;
    const int maxGenerations = 50;
    const double minDistance = 100;
    const double maxDistance = 500;
    const double centerCoords = 300;

    const std::string exportFile = "visuals/routes.csv";

    std::vector<Point>
        depots = {{centerCoords, centerCoords}};
    std::vector<Point> customers = getRandomPoints(numCustomers, minDistance, maxDistance);
    std::vector<std::vector<double>> distanceMatrix = getDistanceMatrix(depots, customers);

    std::vector<std::vector<int>> genRoutes = genetic_solver(distanceMatrix, maxPackages, populationSize, maxGenerations);

    std::vector<Point> locations(numDepots + numCustomers);
    std::copy(depots.begin(), depots.end(), locations.begin());
    std::copy(customers.begin(), customers.end(), locations.begin() + numDepots);
    exportMatrixToCSV(genRoutes, locations, exportFile);

    /* Working Clarke-Wright stuff

    auto [routesByIndex, routesProgress] = clarkeWrightSolver(distanceMatrix, maxPackages);

    exportMatrixToCSV(routesByIndex, locations, exportFile);

    for (const auto &route : routesByIndex)
    {
        for (const auto p : route)
        {
            std::cout << p << " ";
        }
        std::cout << std::endl;
    }

    // Add the depot to the routes in routesProgress
    for (auto &routes : routesProgress)
    {
        for (auto &route : routes)
        {
            route.insert(route.begin(), 0);
            route.push_back(0);
        }
    }

    // Print routes progress
    for (const auto &routes : routesProgress)
    {
        for (const auto &route : routes)
        {
            for (const auto p : route)
            {
                std::cout << p << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }

    // Export routes progress to csv
    exportRoutesProgressToCSV(routesProgress, locations, exportFile);
    */

    return 0;
}