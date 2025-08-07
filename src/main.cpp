#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include <limits>
#include <cmath>
#include <omp.h>

#include "utils.h"
#include "clarke_wright.h"
#include "genetic_algorithm.h"
#include "api_solvers.h"

int main()
{
    const size_t numCustomers = 50;
    const size_t numDepots = 1;
    const size_t maxPackages = 10;
    const size_t populationSize = 100;
    const size_t maxGenerations = 1000;
    const float mutationProb = 0.5;
    const double minDistance = 100;
    const double maxDistance = 1000;
    const double centerCoords = 550;

    const std::string exportFile = "../example/routes.csv";

    std::vector<Point> depots = {{centerCoords, centerCoords}};
    std::vector<Point> customers = getRandomPoints(numCustomers, minDistance, maxDistance);
    Matrix distanceMatrix = getDistanceMatrix(depots, customers);

    std::vector<double> locations_x;
    std::vector<double> locations_y;

    for (const auto &customer : customers)
    {
        locations_x.push_back(customer.x);
        locations_y.push_back(customer.y);
    }

    std::vector<std::vector<std::vector<int>>> clarkeWrightProgression = completeSolverClarkeWright(
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

    std::vector<Point> locations(numDepots + numCustomers);
    std::copy(depots.begin(), depots.end(), locations.begin());
    std::copy(customers.begin(), customers.end(), locations.begin() + numDepots);
    exportRoutesProgressToCSV(clarkeWrightProgression, locations, exportFile);

    return 0;
}