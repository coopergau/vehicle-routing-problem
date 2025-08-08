#include "utils.h"
#include "clarke_wright.h"
#include "genetic_algorithm.h"
#include <vector>
#include <stdexcept>

// Functions to be accessed in Python

std::vector<std::vector<std::vector<int>>> completeSolverClarkeWright(
    const double &depot_x,
    const double &depot_y,
    const std::vector<double> &customers_x,
    const std::vector<double> &customers_y,
    const size_t maxPackages,
    const bool exportData,
    const std::string &filename = "")
{
    if (customers_x.size() != customers_y.size())
    {
        throw std::invalid_argument("locations_x and locations_y must be the same length.");
    }

    std::vector<Point> depots = {{depot_x, depot_y}};

    std::vector<Point> customers;
    customers.reserve(customers_x.size());
    for (size_t i = 0; i < customers_x.size(); ++i)
    {
        Point customer = {customers_x[i], customers_y[i]};
        customers.push_back(customer);
    }

    Matrix distanceMatrix = getDistanceMatrix(depots, customers);
    auto [routesByIndex, routesProgress] = clarkeWrightSolver(distanceMatrix, maxPackages);

    if (exportData)
    {
        std::vector<Point>
            locations(depots.size() + customers.size());
        std::copy(depots.begin(), depots.end(), locations.begin());
        std::copy(customers.begin(), customers.end(), locations.begin() + depots.size());
        exportRoutesProgressToCSV(routesProgress, locations, filename);
    }

    return routesProgress;
}

std::vector<std::vector<std::vector<int>>> completeSolverGenetic(
    const double &depot_x,
    const double &depot_y,
    const std::vector<double> &customers_x,
    const std::vector<double> &customers_y,
    const size_t maxPackages,
    const size_t populationSize,
    const size_t generations,
    const float mutationProb,
    const bool exportData,
    const StartingType startingType = StartingType::ClarkeWright,
    const std::string &filename = "")
{
    if (customers_x.size() != customers_y.size())
    {
        throw std::invalid_argument("locations_x and locations_y must be the same length.");
    }

    std::vector<Point> depots = {{depot_x, depot_y}};

    std::vector<Point> customers;
    customers.reserve(customers_x.size());
    for (size_t i = 0; i < customers_x.size(); ++i)
    {
        Point customer = {customers_x[i], customers_y[i]};
        customers.push_back(customer);
    }

    Matrix distanceMatrix = getDistanceMatrix(depots, customers);

    std::vector<std::vector<std::vector<int>>> routesProgress = geneticSolver(
        distanceMatrix, maxPackages, populationSize, generations, mutationProb, startingType);

    if (exportData)
    {
        std::vector<Point>
            locations(depots.size() + customers.size());
        std::copy(depots.begin(), depots.end(), locations.begin());
        std::copy(customers.begin(), customers.end(), locations.begin() + depots.size());
        exportRoutesProgressToCSV(routesProgress, locations, filename);
    }

    return routesProgress;
}