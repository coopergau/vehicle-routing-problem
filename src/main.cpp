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

void visualiseRoutes(sf::RenderWindow &window,
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

void animateRoutes(sf::RenderWindow &window,
                   const std::vector<Point> &customers,
                   const std::vector<Point> &depots,
                   const std::vector<std::vector<std::vector<int>>> &routesProgress)
{
    // Blue nodes are customers, red nodes are depots
    sf::CircleShape blueNode(5);
    blueNode.setFillColor(sf::Color::Blue);
    sf::CircleShape redNode(5);
    redNode.setFillColor(sf::Color::Red);

    const std::vector<sf::Color> routeColors = {
        sf::Color(255, 0, 0),     // Red
        sf::Color(0, 128, 0),     // Green
        sf::Color(0, 0, 255),     // Blue
        sf::Color(255, 165, 0),   // Orange
        sf::Color(128, 0, 128),   // Purple
        sf::Color(0, 128, 128),   // Teal
        sf::Color(139, 69, 19),   // Brown
        sf::Color(255, 192, 203), // Pink
        sf::Color(0, 0, 0),       // Black
        sf::Color(128, 128, 128)  // Gray
    };

    // For each step in the algorithm
    for (size_t step = 0; step < routesProgress.size(); step++)
    {
        window.clear(sf::Color::White);

        std::vector<std::vector<Point>> pointRoutes = routeIndicesToLocations(routesProgress[step], depots, customers);

        for (size_t routeIdx = 0; routeIdx < pointRoutes.size(); routeIdx++)
        {
            const auto &route = pointRoutes[routeIdx];

            sf::Color routeColor = routeColors[routeIdx % routeColors.size()];

            sf::VertexArray path(sf::LinesStrip, route.size());
            for (size_t i = 0; i < route.size(); i++)
            {
                path[i].position = sf::Vector2f(route[i].x, route[i].y);
                path[i].color = routeColor;
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
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
};

int main()
{
    const int numCustomers = 55;
    const int numDepots = 1;
    const int maxPackages = 10;
    const double minDistance = 100.0;
    const double maxDistance = 500.0;
    const double centerCoords = 300;

    const std::string exportFile = "visuals/routes.csv";

    std::vector<Point>
        depots = {{centerCoords, centerCoords}};
    std::vector<Point> customers = getRandomPoints(numCustomers, minDistance, maxDistance);
    std::vector<std::vector<double>> distanceMatrix = getDistanceMatrix(depots, customers);

    auto [routesByIndex, RoutesProgress] = clarkeWrightSolver(distanceMatrix, maxPackages);

    std::vector<Point> locations(numDepots + numCustomers);
    std::copy(depots.begin(), depots.end(), locations.begin());
    std::copy(customers.begin(), customers.end(), locations.begin() + numDepots);
    exportMatrixToCSV(routesByIndex, locations, exportFile);

    for (const auto &route : routesByIndex)
    {
        for (const auto p : route)
        {
            std::cout << p << " ";
        }
        std::cout << std::endl;
    }

    // Add the depot to the routes in routesProgress
    for (auto &routes : RoutesProgress)
    {
        for (auto &route : routes)
        {
            route.insert(route.begin(), 0);
            route.push_back(0);
        }
    }

    // Visuals
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

        // std::vector<std::vector<Point>> routes = routeIndicesToLocations(routesByIndex, depots, customers);
        // visualiseRoutes(window, customers, depots, routes);
        animateRoutes(window, customers, depots, RoutesProgress);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }

    return 0;
}