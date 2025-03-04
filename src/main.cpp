#include <iostream>
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>

// Struct representing the position of a point that would be a drop off location.
struct Point
{
    double x, y;
};

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

void visualiseVrp(sf::RenderWindow &window, std::vector<Point> &customers, std::vector<Point> &warehouses)
{
    // Blue nodes are customers, red nodes are warehouses
    sf::CircleShape blueNode(5);
    blueNode.setFillColor(sf::Color::Blue);
    sf::CircleShape redNode(5);
    redNode.setFillColor(sf::Color::Red);

    for (auto &customer : customers)
    {
        blueNode.setPosition(customer.x - 5, customer.y - 5);
        window.draw(blueNode);
    }
    for (auto &warehouse : warehouses)
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
    const int numWarehouses = 3;
    const double minDistance = 0.0;
    const double maxDistance = 500.0;

    std::vector<Point> customers = getRandomPoints(NumCustomers, minDistance, maxDistance);
    std::vector<Point> warehouses = getRandomPoints(numWarehouses, minDistance, maxDistance);

    for (const auto &point : customers)
    {
        std::cout << point.x << ", " << point.y << "\n";
    }
    std::cout << "Warehouses\n";
    for (const auto &point : warehouses)
    {
        std::cout << point.x << ", " << point.y << "\n";
    }

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
        visualiseVrp(window, customers, warehouses);
    }

    return 0;
}