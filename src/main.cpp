#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include <limits>
#include <cmath>

#include "utils.h"
#include "clarke_wright.h"
#include "genetic_algorithm.h"
#include "api_solvers.h"

int main()
{
    const size_t numCustomers = 80;
    const size_t numDepots = 1;
    const size_t maxPackages = 10;
    const size_t populationSize = 100;
    const size_t maxGenerations = 1000;
    const float mutationProb = 0.5;
    const double minDistance = 100;
    const double maxDistance = 1000;
    const double centerCoords = 550;

    const std::string exportFile = "../example/routes.csv";

    std::vector<double> locations_x = {638, 471.562, 708.806, 797.317, 299.051, 533.925, 453.046, 885.210, 695.615, 314.599, 752.461, 988.943, 626.947, 930.201, 599.925, 189.84, 275.959, 457.436, 931.662, 948.443, 419.236, 869.817, 947.822, 547.613, 770.531, 219.516, 153.438, 318.300, 403.169, 522.098, 794.861, 132.592, 243.351, 981.814, 359};
    std::vector<double> locations_y = {504, 452.871, 463.492, 778.824, 947.275, 940.909, 702.878, 831.618, 570.149, 342.502, 264.979, 530.139, 240.031, 146.053, 575.459, 865.74, 469.273, 202.455, 179.411, 772.261, 764.169, 672.552, 970.377, 286.836, 186.545, 278.056, 881.281, 623.291, 411.491, 561.256, 964.085, 170.396, 138.856, 916.693, 557};

    std::vector<std::vector<std::vector<int>>> clarkeWrightSolution = completeSolverClarkeWright(
        centerCoords,
        centerCoords,
        locations_x,
        locations_y,
        maxPackages,
        true,
        exportFile);
    /*std::vector<std::vector<std::vector<int>>> geneticSolution = completeSolverGenetic(
        centerCoords,
        centerCoords,
        locations_x,
        locations_y,
        maxPackages,
        populationSize,
        maxGenerations,
        mutationProb,
        true,
        StartingType::NearestNeighbours,
        exportFile);*/

    /*std::vector<Point>
        depots = {{centerCoords, centerCoords}};
    std::vector<Point> customers = getRandomPoints(numCustomers, minDistance, maxDistance);
    Matrix distanceMatrix = getDistanceMatrix(depots, customers);

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<std::vector<int>>> genRoutesProgress = geneticSolver(distanceMatrix, maxPackages, populationSize, maxGenerations, mutationProb, StartingType::NearestNeighbours);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;

    std::vector<Point>
        locations(numDepots + numCustomers);
    std::copy(depots.begin(), depots.end(), locations.begin());
    std::copy(customers.begin(), customers.end(), locations.begin() + numDepots);
    exportRoutesProgressToCSV(genRoutesProgress, locations, exportFile);*/

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