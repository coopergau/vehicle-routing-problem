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
    const size_t numCustomers = 80;
    const size_t numDepots = 1;
    const size_t maxPackages = 10;
    const size_t populationSize = 100;
    const size_t maxGenerations = 1000;
    const double minDistance = 100;
    const double maxDistance = 1000;
    const double centerCoords = 550;

    const std::string exportFile = "visuals/routes.csv";

    std::vector<Point>
        depots = {{centerCoords, centerCoords}};
    std::vector<Point> customers = getRandomPoints(numCustomers, minDistance, maxDistance);
    Matrix distanceMatrix = getDistanceMatrix(depots, customers);

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<std::vector<int>>> genRoutesProgress = genetic_solver(distanceMatrix, maxPackages, populationSize, maxGenerations);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;

    std::vector<Point>
        locations(numDepots + numCustomers);
    std::copy(depots.begin(), depots.end(), locations.begin());
    std::copy(customers.begin(), customers.end(), locations.begin() + numDepots);
    exportRoutesProgressToCSV(genRoutesProgress, locations, exportFile);

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