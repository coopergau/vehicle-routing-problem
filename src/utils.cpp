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
std::vector<Point> getRandomPoints(const size_t numPoints, const double minDistance, const double maxDistance)
{
    std::vector<Point> points;
    points.reserve(numPoints);

    // rng
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(minDistance, maxDistance);

    for (int i = 0; i < numPoints; i++)
    {
        points.push_back({dist(gen), dist(gen)});
    }

    return points;
}

// Generates a square distance matrix where the value at row i and col j is the distance between points i and j.
Matrix getDistanceMatrix(const std::vector<Point> &depots, const std::vector<Point> &customers)
{
    int numDepots = depots.size();
    int numCustomers = customers.size();
    int matrixSize = numDepots + numCustomers;

    Matrix distanceMatrix;
    distanceMatrix.data = std::vector<double>(matrixSize * matrixSize, 0.0);
    distanceMatrix.rows = std::vector<double *>(matrixSize);

    for (int i = 0; i < matrixSize; ++i)
    {
        distanceMatrix.rows[i] = &distanceMatrix.data[i * matrixSize];
    }

    // Combine all points into one vec for iteration
    std::vector<Point> allLocations;
    allLocations.reserve(matrixSize);
    allLocations.insert(allLocations.end(), depots.begin(), depots.end());
    allLocations.insert(allLocations.end(), customers.begin(), customers.end());

    // Fill the matrix with the distances between locations
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            if (i == j)
            {
                // Going from one location to itself is not a valid path
                distanceMatrix.rows[i][j] = std::numeric_limits<double>::infinity();
            }
            else if (i < numDepots && j < numDepots)
            {
                // Going from one depot to another depot is not a valid path
                distanceMatrix.rows[i][j] = std::numeric_limits<double>::infinity();
            }
            else
            {
                double x_dist = allLocations[j].x - allLocations[i].x;
                double y_dist = allLocations[j].y - allLocations[i].y;
                distanceMatrix.rows[i][j] = std::sqrt((x_dist * x_dist) + (y_dist * y_dist));
            }
        }
    }

    return distanceMatrix;
}

void exportMatrixToCSV(const std::vector<std::vector<int>> &routes, const std::vector<Point> &locations, const std::string &filename)
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

void exportRoutesProgressToCSV(const std::vector<std::vector<std::vector<int>>> &routesProgress, const std::vector<Point> &locations, const std::string &filename)
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