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

std::vector<Point> getRandomPoints(int count, double minDistance, double maxDistance);
std::vector<std::vector<double>> getDistanceMatrix(const std::vector<Point> &depots, const std::vector<Point> &customers);
std::vector<std::vector<Point>> routeIndicesToLocations(
    std::vector<std::vector<int>> routesByIndex,
    std::vector<Point> depots,
    std::vector<Point> customers);
void exportMatrixToCSV(std::vector<std::vector<int>> &routes, std::vector<Point> &locations, const std::string &filename);
void exportRoutesProgressToCSV(std::vector<std::vector<std::vector<int>>> &routesProgress, std::vector<Point> &locations, const std::string &filename);

#endif