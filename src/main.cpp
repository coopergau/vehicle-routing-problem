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

    /*std::vector<std::vector<std::vector<int>>> clarkeWrightSolution = completeSolverClarkeWright(
        centerCoords,
        centerCoords,
        locations_x,
        locations_y,
        maxPackages,
        true,
        exportFile);*/

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

    // Comparing with and without parellel computation
    std::vector<double> sequentialTimes;
    std::vector<double> parallelTimes;
    for (int i = 0; i < 10; ++i)
    {
        std::cout << i << std::endl;
        std::vector<Point> depots = {{centerCoords, centerCoords}};
        std::vector<Point> customers = getRandomPoints(numCustomers, minDistance, maxDistance);
        Matrix distanceMatrix = getDistanceMatrix(depots, customers);

        auto seqStart = std::chrono::high_resolution_clock::now();
        std::vector<std::vector<std::vector<int>>> sequential = geneticSolver(
            distanceMatrix, maxPackages, populationSize, maxGenerations, mutationProb, StartingType::Mixed, false);
        auto seqEnd = std::chrono::high_resolution_clock::now();
        auto seqDuration = std::chrono::duration_cast<std::chrono::milliseconds>(seqEnd - seqStart);
        sequentialTimes.push_back(seqDuration.count());

        std::cout << i << std::endl;

        auto parStart = std::chrono::high_resolution_clock::now();
        std::vector<std::vector<std::vector<int>>> parallel = geneticSolver(
            distanceMatrix, maxPackages, populationSize, maxGenerations, mutationProb, StartingType::Mixed, true);
        auto parEnd = std::chrono::high_resolution_clock::now();
        auto parDuration = std::chrono::duration_cast<std::chrono::milliseconds>(parEnd - parStart);
        parallelTimes.push_back(parDuration.count());
    }

    // Calculate statistics
    double sequential_avg = std::accumulate(sequentialTimes.begin(), sequentialTimes.end(), 0.0) / sequentialTimes.size();
    double parallel_avg = std::accumulate(parallelTimes.begin(), parallelTimes.end(), 0.0) / parallelTimes.size();

    std::cout << "Sequential: " << sequential_avg << "ms" << std::endl;
    std::cout << "Parallel: " << parallel_avg << "ms" << std::endl;
    std::cout << "Speedup: " << sequential_avg / parallel_avg << "x" << std::endl;

    /*std::vector<Point>
        locations(numDepots + numCustomers);
    std::copy(depots.begin(), depots.end(), locations.begin());
    std::copy(customers.begin(), customers.end(), locations.begin() + numDepots);
    exportRoutesProgressToCSV(genRoutesProgress, locations, exportFile);*/

    return 0;
}