#include "utils.h"

#include <vector>
#include <random>
#include <cmath>

// Function generates random points within a given area range.
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

// Function generates distance matrix where the value at row i and col j is the distance
// between points i and j.
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