#include <iostream>
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>

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
    const double minDistance = 0.0;
    const double maxDistance = 500.0;

    std::vector<Point> customers = getRandomPoints(NumCustomers, minDistance, maxDistance);
    std::vector<Point> warehouses = getRandomPoints(numWarehouses, minDistance, maxDistance);

    std::vector<std::vector<Point>> routes(numVehicles);
    routes[0].push_back(warehouses[0]);
    routes[1].push_back(warehouses[1]);
    routes[2].push_back(warehouses[2]);

    for (int i = 0; i < 7; i++)
        routes[0].push_back(customers[i]);
    for (int i = 7; i < 14; i++)
        routes[1].push_back(customers[i]);
    for (int i = 14; i < 20; i++)
        routes[2].push_back(customers[i]);

    routes[0].push_back(warehouses[0]);
    routes[1].push_back(warehouses[1]);
    routes[2].push_back(warehouses[2]);

    sf::RenderWindow window(sf::VideoMode(500, 500), "VRP");

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