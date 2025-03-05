#include <iostream>
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <limits>
#include <cmath>

#include "point.h"

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
std::vector<std::vector<double>> getDistanceMatrix(std::vector<Point> warehouses, std::vector<Point> customers)
{
    int numWarehouses = warehouses.size();
    int numCustomers = customers.size();
    std::vector<std::vector<double>> distanceMatrix(numWarehouses + numCustomers, std::vector<double>(numWarehouses + numCustomers, 0.0));

    // Combine all points into one vec for iteration
    std::vector<Point> allLocations(numWarehouses + numCustomers);
    std::copy(warehouses.begin(), warehouses.end(), allLocations.begin());
    std::copy(customers.begin(), customers.end(), allLocations.begin() + numWarehouses);

    // Fill the matrix with the distances between locations
    for (int i = 0; i < numWarehouses + numCustomers; i++)
    {
        for (int j = 0; j < numWarehouses + numCustomers; j++)
        {
            if (i == j)
            {
                // Going from one location to itself is not a valid path
                distanceMatrix[i][j] = std::numeric_limits<double>::infinity();
            }
            else if (i < numWarehouses && j < numWarehouses)
            {
                // Going from one warehouse to another warehouse is not a valid path
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
                  const std::vector<Point> &warehouses,
                  const std::vector<std::vector<Point>> &routes)
{
    // Blue nodes are customers, red nodes are warehouses
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
    for (const auto &warehouse : warehouses)
    {
        redNode.setPosition(warehouse.x - 5, warehouse.y - 5);
        window.draw(redNode);
    }

    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
};

int main()
{
    const int NumCustomers = 20;
    const int numWarehouses = 1;
    const int numVehicles = 3;
    const double minDistance = 100.0;
    const double maxDistance = 500.0;

    std::vector<Point> warehouses = getRandomPoints(numWarehouses, minDistance, maxDistance);
    std::vector<Point> customers = getRandomPoints(NumCustomers, minDistance, maxDistance);
    std::vector<std::vector<double>> distanceMatrix = getDistanceMatrix(warehouses, customers);

    std::vector<std::vector<Point>> routes(numVehicles);
    for (int i = 0; i < numVehicles; i++)
    {
        routes[i].push_back(warehouses[0]);
    }

    for (int i = 0; i < 7; i++)
        routes[0].push_back(customers[i]);
    for (int i = 7; i < 14; i++)
        routes[1].push_back(customers[i]);
    for (int i = 14; i < 20; i++)
        routes[2].push_back(customers[i]);

    for (int i = 0; i < numVehicles; i++)
    {
        routes[i].push_back(warehouses[0]);
    }

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
        visualiseVrp(window, customers, warehouses, routes);
    }

    return 0;
}