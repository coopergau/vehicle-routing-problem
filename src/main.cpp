#include <iostream>
#include <vector>
#include <random>

// Struct representing the position of a point that would be a drop off location.
struct Point
{
    double x, y;
};

// Function generates random points within a given area range.
std::vector<Point> get_random_points(int num_points, double min_distance, double max_distance)
{
    std::vector<Point> points;

    // Specify rng
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(min_distance, max_distance);

    // Generate points
    for (int i = 0; i < num_points; i++)
    {
        points.push_back({dist(gen), dist(gen)});
    }

    return points;
}

int main()
{
    const int num_customers = 20;
    const int num_warehouses = 3;
    const double min_distance = 0.0;
    const double max_distance = 100.0;

    std::vector<Point> customers = get_random_points(num_customers, min_distance, max_distance);
    std::vector<Point> warehouses = get_random_points(num_warehouses, min_distance, max_distance);

    for (const auto &point : customers)
    {
        std::cout << point.x << ", " << point.y << "\n";
    }
    std::cout << "Warehouses\n";
    for (const auto &point : warehouses)
    {
        std::cout << point.x << ", " << point.y << "\n";
    }
}