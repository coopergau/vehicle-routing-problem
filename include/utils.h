#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

// Struct representing the position of a point that would be a drop off location.
struct Point
{
    double x{};
    double y{};
};
std::ostream &operator<<(std::ostream &os, const Point &point);

// Struct for distance matrix
struct Matrix
{
    std::vector<double> data;
    std::vector<double *> rows;
};

std::vector<Point> getRandomPoints(size_t count, double minDistance, double maxDistance);
Matrix getDistanceMatrix(const std::vector<Point> &depots, const std::vector<Point> &customers);
std::vector<std::vector<Point>> routeIndicesToLocations(
    const std::vector<std::vector<int>> routesByIndex,
    const std::vector<Point> depots,
    const std::vector<Point> customers);
void exportMatrixToCSV(const std::vector<std::vector<int>> &routes, const std::vector<Point> &locations, const std::string &filename);
void exportRoutesProgressToCSV(const std::vector<std::vector<std::vector<int>>> &routesProgress, const std::vector<Point> &locations, const std::string &filename);

#endif