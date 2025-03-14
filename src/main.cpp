#include <iostream>
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <limits>
#include <cmath>

#include "utils.h"
#include "clarke_wright.h"

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
    const int numCustomers = 100;
    const int numDepots = 1;
    const int numVehicles = 3;
    const double minDistance = 100.0;
    const double maxDistance = 500.0;

    // std::vector<Point> depots = getRandomPoints(numDepots, minDistance, maxDistance);
    std::vector<Point> depots = {{300, 300}};
    std::vector<Point> customers = getRandomPoints(numCustomers, minDistance, maxDistance);
    std::vector<std::vector<double>> distanceMatrix = getDistanceMatrix(depots, customers);

    std::vector<std::vector<int>> routesByIndex = clarkeWrightSolver(distanceMatrix);

    for (const auto &route : routesByIndex)
    {
        for (const auto p : route)
        {
            std::cout << p << " ";
        }
        std::cout << std::endl;
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

        std::vector<std::vector<Point>> routes; // temp check if this is optimal

        std::vector<Point> allLocations;
        allLocations.reserve(depots.size() + customers.size()); // Optimize memory allocation
        allLocations.insert(allLocations.end(), depots.begin(), depots.end());
        allLocations.insert(allLocations.end(), customers.begin(), customers.end());

        for (const auto &routeIndices : routesByIndex) // Loop over each route
        {
            std::vector<Point> route;
            for (int index : routeIndices)
            {
                route.push_back(allLocations[index]);
            }
            routes.push_back(route);
        }

        visualiseVrp(window, customers, depots, routes); // need to make routes a vec of vecs of points
    }

    return 0;
}