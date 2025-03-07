#include <iostream>
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <limits>
#include <cmath>

#include "point.h"
#include "clarke_wright.h"

// Function generates random points within a given area range.
std::vector<Point> getRandomPoints(int numPoints, double minDistance, double maxDistance)
{
    std::vector<Point> points;

    // Specify rng
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

void visualiseVrp(sf::RenderWindow &window,
                  const std::vector<Point> &customers,
                  const std::vector<Point> &depots,
                  const std::vector<std::vector<Point>> &routes)
{
    // Blue nodes are customers, red nodes are depots
    sf::CircleShape blueNode(5);
    blueNode.setFillColor(sf::Color::Blue);
    sf::CircleShape redNode(5);
    redNode.setFillColor(sf::Color::Red);

    for (const auto &route : routes)
    {
        sf::VertexArray path(sf::LinesStrip, route.size());
        for (size_t i = 0; i < route.size(); i++)
        {
            path[i].position = sf::Vector2f(route[i].x, route[i].y);
            path[i].color = sf::Color::Black;
        }
        window.draw(path);
    }

    for (const auto &customer : customers)
    {
        blueNode.setPosition(customer.x - 5, customer.y - 5);
        window.draw(blueNode);
    }
    for (const auto &depot : depots)
    {
        redNode.setPosition(depot.x - 5, depot.y - 5);
        window.draw(redNode);
    }

    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
};

int main()
{
    const int NumCustomers = 20;
    const int numDepots = 1;
    const int numVehicles = 3;
    const double minDistance = 100.0;
    const double maxDistance = 500.0;

    std::vector<Point> depots = getRandomPoints(numDepots, minDistance, maxDistance);
    std::vector<Point> customers = getRandomPoints(NumCustomers, minDistance, maxDistance);
    std::vector<std::vector<double>> distanceMatrix = getDistanceMatrix(depots, customers);

    std::vector<std::vector<int>> routes = clarkeWrightSolver(distanceMatrix);

    sf::RenderWindow window(sf::VideoMode(600, 600), "VRP");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        // visualiseVrp(window, customers, depots, routes); need to make routes a vec of vecs of points
    }

    return 0;
}