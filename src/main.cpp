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

std::vector<std::vector<Point>> routeIndicesToLocations(
    std::vector<std::vector<int>> routesByIndex,
    std::vector<Point> depots,
    std::vector<Point> customers)
{
    // Take the routes by index and map them to their actual locations to get routes by location.
    std::vector<Point> allLocations;
    allLocations.reserve(depots.size() + customers.size());
    allLocations.insert(allLocations.end(), depots.begin(), depots.end());
    allLocations.insert(allLocations.end(), customers.begin(), customers.end());

    std::vector<std::vector<Point>> routes;

    for (const auto &routeIndices : routesByIndex)
    {
        std::vector<Point> route;
        for (int index : routeIndices)
        {
            route.push_back(allLocations[index]);
        }
        routes.push_back(route);
    }

    return routes;
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
    const int numCustomers = 20;
    const int numDepots = 1;
    const int maxPackages = 10;
    const double minDistance = 100.0;
    const double maxDistance = 500.0;
    const double centerCoords = 300;

    // std::vector<Point> depots = getRandomPoints(numDepots, minDistance, maxDistance);
    std::vector<Point> depots = {{centerCoords, centerCoords}};
    std::vector<Point> customers = getRandomPoints(numCustomers, minDistance, maxDistance);
    std::vector<std::vector<double>> distanceMatrix = getDistanceMatrix(depots, customers);

    std::vector<std::vector<int>> routesByIndex = clarkeWrightSolver(distanceMatrix, maxPackages);

    for (const auto &route : routesByIndex)
    {
        for (const auto p : route)
        {
            std::cout << p << " ";
        }
        std::cout << std::endl;
    }

    std::vector<std::vector<Point>> routes = routeIndicesToLocations(routesByIndex, depots, customers);

    sf::RenderWindow window(sf::VideoMode(600, 600), "Routes");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        visualiseVrp(window, customers, depots, routes);
    }

    return 0;
}