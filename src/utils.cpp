#include "utils.h"

#include <vector>
#include <random>
#include <cmath>
#include <string>
#include <fstream>

// For writing Point types to csv
std::ostream &operator<<(std::ostream &os, const Point &point)
{
    os << point.x << "," << point.y;
    return os;
}

// Generates random points within a given area range.
std::vector<Point> getRandomPoints(int numPoints, double minDistance, double maxDistance)
{
    std::vector<Point> points;

    // rng
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(minDistance, maxDistance);

    // Generate points
    for (int i = 0; i < numPoints; i++)
    {
        points.push_back({dist(gen), dist(gen)});
    }

    return points;
}

// Generates distance matrix where the value at row i and col j is the distance between points i and j.
std::vector<std::vector<double>> getDistanceMatrix(const std::vector<Point> &depots, const std::vector<Point> &customers)
{
    int numDepots = depots.size();
    int numCustomers = customers.size();
    std::vector<std::vector<double>> distanceMatrix(numDepots + numCustomers, std::vector<double>(numDepots + numCustomers, 0.0));

    // Combine all points into one vec for iteration
    std::vector<Point> allLocations(numDepots + numCustomers);
    std::copy(depots.begin(), depots.end(), allLocations.begin());
    std::copy(customers.begin(), customers.end(), allLocations.begin() + numDepots);

    // Fill the matrix with the distances between locations
    for (int i = 0; i < numDepots + numCustomers; i++)
    {
        for (int j = 0; j < numDepots + numCustomers; j++)
        {
            if (i == j)
            {
                // Going from one location to itself is not a valid path
                distanceMatrix[i][j] = std::numeric_limits<double>::infinity();
            }
            else if (i < numDepots && j < numDepots)
            {
                // Going from one depot to another depot is not a valid path
                distanceMatrix[i][j] = std::numeric_limits<double>::infinity();
            }
            else
            {
                double x_dist = allLocations[j].x - allLocations[i].x;
                double y_dist = allLocations[j].y - allLocations[i].y;
                distanceMatrix[i][j] = std::sqrt((x_dist * x_dist) + (y_dist * y_dist));
            }
        }
    }

    return distanceMatrix;
}

// Take the routes by index and map them to their actual location coordinates to get routes by location.
std::vector<std::vector<Point>> routeIndicesToLocations(
    std::vector<std::vector<int>> routesByIndex,
    std::vector<Point> depots,
    std::vector<Point> customers)
{
    std::vector<Point> allLocations;
    allLocations.reserve(depots.size() + customers.size());
    allLocations.insert(allLocations.end(), depots.begin(), depots.end());
    allLocations.insert(allLocations.end(), customers.begin(), customers.end());

    std::vector<std::vector<Point>> routes;

    for (const auto &routeIndices : routesByIndex)
    {
        std::vector<Point> route;
        for (int index : routeIndices)
        {
            route.push_back(allLocations[index]);
        }
        routes.push_back(route);
    }

    return routes;
}

void exportMatrixToCSV(std::vector<std::vector<int>> &routes, std::vector<Point> &locations, const std::string &filename)
{
    std::ofstream file(filename);

    // Write the locations
    for (const auto location : locations)
    {
        file << location;
        file << ",";
    }
    file << "\n";

    // Write the routes
    for (const auto &route : routes)
    {
        for (const auto customer : route)
        {
            file << customer;
            file << ",";
        }
        file << "\n";
    }
    file << "END";
    file.close();
}

void exportRoutesProgressToCSV(std::vector<std::vector<std::vector<int>>> &routesProgress, std::vector<Point> &locations, const std::string &filename)
{
    std::ofstream file(filename);

    // Write the locations
    for (const auto location : locations)
    {
        file << location;
        file << ",";
    }
    file << "\n";

    // Write the routes
    for (const auto &routes : routesProgress)
    {
        for (const auto &route : routes)
        {
            for (const auto customer : route)
            {
                file << customer;
                file << ",";
            }
            file << "\n";
        }
        file << "END";
        file << "\n";
    }
    file.close();
}