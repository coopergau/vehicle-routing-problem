#include <catch2/catch_test_macros.hpp>
#include <random>

#include "utils.h"
#include "clarke_wright.h"

#include <iostream>
#include <sstream>

/* Fuzz test checks that:
    1. The routes generated include every customer (represented as their index) exactly once.
    2. Each route starts and ends with zero (depot).
    3. Zero does not occur anywhere else.
    4. No route is longer than the max length (maxPackages).
*/
TEST_CASE("Fuzz test that clarkeWrightSolver returns a proper solution", "[clarkeWrightSolver]")
{
    const double minDistance = 100.0;
    const double maxDistance = 500.0;
    const size_t numDepots = 1;

    size_t fuzzRounds = 50;

    size_t minCustomers = 5;
    size_t maxCustomers = 200;

    size_t minMaxPackages = 5;
    size_t maxMaxPackages = 15;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> customerDist(minCustomers, maxCustomers);
    std::uniform_int_distribution<size_t> maxPackagesDist(minMaxPackages, maxMaxPackages);

    for (size_t i = 0; i < fuzzRounds; ++i)
    {
        const size_t numCustomers = customerDist(gen);
        const size_t maxPackages = maxPackagesDist(gen);

        std::vector<Point> depots = getRandomPoints(numDepots, minDistance, maxDistance);
        std::vector<Point> customers = getRandomPoints(numCustomers, minDistance, maxDistance);
        Matrix distanceMatrix = getDistanceMatrix(depots, customers);

        auto [routes, routesProgress] = clarkeWrightSolver(distanceMatrix, maxPackages);

        // Count occurences of each index
        std::vector<int> count(numCustomers + 1, 0);
        for (const auto &route : routes)
        {
            for (const auto customer : route)
            {
                REQUIRE(customer < numCustomers + 1);
                count[customer]++;
            }
        }

        // Check that positive nums occur exactly once
        for (int i = 1; i <= numCustomers; i++)
        {
            if (count[i] != 1)
            {
                std::ostringstream oss;
                for (const auto &route : routes)
                {
                    for (const auto customer : route)
                        oss << customer << " ";
                    oss << "\n";
                }
                INFO("Routes:\n"
                     << oss.str());
                REQUIRE(count[i] == 1);
            }
        }

        // Check that each route starts and ends with zero
        for (const auto &route : routes)
        {
            REQUIRE(route.front() == 0);
            REQUIRE(route.back() == 0);
        }

        // Check that zero occurs nowhere else
        REQUIRE(count[0] == routes.size() * 2);

        // Check that each route does not violate max length
        for (const auto &route : routes)
        {
            REQUIRE(route.size() <= maxPackages + 2); // + 2 to account for the zero at beginning and end.
        }
    }
}

TEST_CASE("Process savings makes single routes", "[processSavings]")
{
    std::vector<std::tuple<int, int, double>> savings = {
        {1, 2, 10.0},
        {3, 4, 9.0}};
    int numCustomers = 4;
    int maxPackages = 100;

    auto [actualRoutes, routesProgress] = processSavings(savings, numCustomers, maxPackages);

    std::vector<std::vector<int>> expectedRoutes = {{0, 1, 2, 0},
                                                    {0, 3, 4, 0}};

    REQUIRE(actualRoutes == expectedRoutes);
}

TEST_CASE("Process savings joins single element to route", "[processSavings]")
{
    std::vector<std::tuple<int, int, double>> savings = {
        {1, 2, 10.0},
        {3, 4, 9.0},
        {5, 1, 8.0},
        {6, 4, 8.0}};
    int numCustomers = 6;
    int maxPackages = 100;

    auto [actualRoutes, routesProgress] = processSavings(savings, numCustomers, maxPackages);

    std::vector<std::vector<int>> expectedRoutes = {{0, 5, 1, 2, 0},
                                                    {0, 3, 4, 6, 0}};

    REQUIRE(actualRoutes == expectedRoutes);
}

TEST_CASE("Process savings joins routes", "[processSavings]")
{
    std::vector<std::tuple<int, int, double>> savings = {
        {1, 2, 10.0},
        {3, 4, 9.0},
        {2, 3, 8.0}};
    int numCustomers = 4;
    int maxPackages = 100;

    auto [actualRoutes, routesProgress] = processSavings(savings, numCustomers, maxPackages);

    std::vector<std::vector<int>> expectedRoutes = {{0, 1, 2, 3, 4, 0}};

    REQUIRE(actualRoutes == expectedRoutes);
}

TEST_CASE("Process savings adds elements to two routes and then joins them", "[processSavings]")
{
    std::vector<std::tuple<int, int, double>> savings = {
        {1, 2, 10.0},
        {3, 2, 9.0},
        {1, 4, 8.0},
        {5, 6, 7.0},
        {7, 6, 6.0},
        {7, 4, 5.0}};
    int numCustomers = 7;
    int maxPackages = 100;

    auto [actualRoutes, routesProgress] = processSavings(savings, numCustomers, maxPackages);

    std::vector<std::vector<int>> expectedRoutes = {{0, 5, 6, 7, 4, 1, 2, 3, 0}};

    REQUIRE(actualRoutes == expectedRoutes);
}

TEST_CASE("Example savings list from 15 random points is processsed correctly", "[processSavings]")
{
    std::vector<std::tuple<int, int, double>> savings = {
        {2, 6, 280.264},
        {3, 14, 274.769},
        {3, 7, 273.273},
        {8, 12, 266.216},
        {10, 13, 250.653},
        {8, 13, 245.774},
        {9, 12, 245.383},
        {5, 9, 243.649},
        {1, 13, 232.455},
        {7, 14, 228.153},
        {1, 15, 206.368},
        {6, 11, 203.126},
        {8, 10, 200.731},
        {2, 11, 197.703},
        {5, 12, 195.931},
        {8, 9, 186.519},
        {6, 14, 185.884},
        {12, 13, 179.619},
        {2, 14, 171.8},
        {1, 3, 159.217},
        {2, 5, 159.007},
        {1, 10, 158.439},
        {13, 15, 157.825},
        {3, 6, 151.291},
        {10, 12, 149.976},
        {11, 14, 149.625},
        {1, 7, 146.216},
        {5, 8, 139.911},
        {2, 3, 136.976},
        {10, 15, 127.844},
        {5, 6, 127.007},
        {6, 7, 121.81},
        {3, 11, 120.902},
        {2, 9, 112.467},
        {9, 13, 109.207},
        {2, 7, 107.476},
        {1, 8, 106.898},
        {3, 15, 103.658},
        {7, 15, 102.463},
        {7, 11, 102.328},
        {1, 14, 100.826},
        {5, 11, 99.1263},
        {9, 10, 96.1487},
        {6, 9, 86.238},
        {8, 15, 82.4222},
        {4, 12, 79.9183},
        {4, 9, 77.0542},
        {4, 8, 74.0972},
        {2, 12, 71.7397},
        {5, 13, 69.6148},
        {4, 5, 68.2484},
        {14, 15, 67.5355},
        {9, 11, 67.2609},
        {5, 10, 63.0542},
        {1, 12, 61.0014},
        {3, 13, 54.6157},
        {4, 10, 53.8217},
        {7, 13, 53.7612},
        {4, 13, 52.3005},
        {6, 12, 50.6683},
        {12, 15, 48.6712},
        {5, 14, 47.6581},
        {11, 12, 38.4954},
        {2, 8, 35.4254},
        {7, 10, 35.3035},
        {3, 10, 33.7446},
        {2, 4, 29.4431},
        {1, 6, 25.6909},
        {3, 5, 25.6486},
        {1, 9, 24.4202},
        {13, 14, 23.2019},
        {9, 14, 22.7716},
        {4, 6, 22.4929},
        {1, 11, 22.1279},
        {6, 8, 21.658},
        {9, 15, 21.1384},
        {4, 11, 19.7941},
        {1, 4, 19.1806},
        {4, 15, 19.0848},
        {5, 7, 17.6915},
        {1, 2, 16.5508},
        {8, 11, 15.9827},
        {6, 15, 15.9036},
        {11, 15, 14.6386},
        {10, 14, 13.2201},
        {2, 15, 9.51055},
        {7, 8, 8.26797},
        {3, 9, 7.93299},
        {1, 5, 7.38836},
        {5, 15, 7.25877},
        {3, 8, 6.34818},
        {12, 14, 5.32776},
        {7, 9, 4.4385},
        {2, 10, 4.4167},
        {4, 14, 3.14491},
        {2, 13, 2.8672},
        {6, 10, 0.985082},
        {10, 11, 0.534217},
        {6, 13, 0.193943},
        {3, 4, 0.184046},
        {7, 12, 0.153177},
        {8, 14, 0.0672766},
        {11, 13, 0.0402312},
        {3, 12, 0.0253871},
        {4, 7, 0.00501565},
    };
    int numCustomers = 15;
    int maxPackages = 100;

    auto [actualRoutes, routesProgress] = processSavings(savings, numCustomers, maxPackages);

    std::vector<std::vector<int>> expectedRoutes = {
        {0, 4, 5, 9, 12, 8, 13, 10, 1, 15, 7, 3, 14, 2, 6, 11, 0},
    };

    REQUIRE(actualRoutes == expectedRoutes);
}