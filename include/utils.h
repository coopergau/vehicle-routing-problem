#ifndef UTILS_H
#define UTILS_H

#include <vector>

// Struct representing the position of a point that would be a drop off location.
struct Point
{
    double x;
    double y;
};

// Functions for generating random locations and their corresponding distance matrix.
std::vector<Point> getRandomPoints(int count, double minDistance, double maxDistance);
std::vector<std::vector<double>> getDistanceMatrix(const std::vector<Point> &depots, const std::vector<Point> &customers);

#endif