#include "genetic_algorithm.h"
#include "utils.h"
#include <catch2/catch_test_macros.hpp>
#include <random>
#include <sstream>

/* Fuzz test checks that:
    1. The routes generated include every customer (represented as their index) exactly once.
    2. Each route starts and ends with zero (depot).
    3. Zero does not occur anywhere else.
    4. No route is longer than the max length (maxPackages).
*/
TEST_CASE("Fuzz test that geneticSolver returns a proper solution", "[geneticSolver]")
{
    const double minDistance = 100.0;
    const double maxDistance = 500.0;
    const size_t numDepots = 1;

    size_t fuzzRounds = 200;

    size_t minCustomers = 5;
    size_t maxCustomers = 200;

    size_t minMaxPackages = 5;
    size_t maxMaxPackages = 15;

    size_t minPopulationSize = 5;
    size_t maxPopulationSize = 30;

    size_t minGenerations = 5;
    size_t maxGenerations = 30;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> customerDist(minCustomers, maxCustomers);
    std::uniform_int_distribution<size_t> maxPackagesDist(minMaxPackages, maxMaxPackages);
    std::uniform_int_distribution<size_t> populationSizeDist(minPopulationSize, maxPopulationSize);
    std::uniform_int_distribution<size_t> generationsDist(minGenerations, maxGenerations);
    std::uniform_real_distribution<float> mutationDist(0.0f, 1.0f);
    std::uniform_int_distribution<int> startingTypeDist(0, static_cast<int>(StartingType::COUNT) - 1);

    for (size_t i = 0; i < fuzzRounds; ++i)
    {
        const size_t numCustomers = customerDist(gen);
        const size_t maxPackages = maxPackagesDist(gen);
        const size_t populationSize = populationSizeDist(gen);
        const size_t generations = generationsDist(gen);
        const float mutationProb = mutationDist(gen);
        StartingType randomType = static_cast<StartingType>(startingTypeDist(gen));

        std::vector<Point> depots = getRandomPoints(numDepots, minDistance, maxDistance);
        std::vector<Point> customers = getRandomPoints(numCustomers, minDistance, maxDistance);
        Matrix distanceMatrix = getDistanceMatrix(depots, customers);

        std::vector<std::vector<std::vector<int>>> genRoutesProgress = geneticSolver(
            distanceMatrix, maxPackages, populationSize, generations, mutationProb, randomType);

        std::vector<std::vector<int>> finalRoutes = genRoutesProgress.back();

        // Count occurences of each index
        std::vector<int> count(numCustomers + 1, 0);
        for (const auto &route : finalRoutes)
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
                for (const auto &route : finalRoutes)
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
        for (const auto &route : finalRoutes)
        {
            REQUIRE(route.front() == 0);
            REQUIRE(route.back() == 0);
        }

        // Check that zero occurs nowhere else
        if (count[0] != finalRoutes.size() * 2)
        {
            std::ostringstream oss;
            oss << "Expected depot count: " << finalRoutes.size() * 2 << "\n";
            oss << "Actual depot count: " << count[0] << "\n";
            for (const auto &route : finalRoutes)
            {
                oss << "Route: ";
                for (auto customer : route)
                {
                    oss << customer << " ";
                }
                oss << "\n";
            }
            INFO(oss.str());
            REQUIRE(count[0] == finalRoutes.size() * 2);
        }

        // Check that each route does not violate max length
        for (const auto &route : finalRoutes)
        {
            REQUIRE(route.size() <= maxPackages + 2); // + 2 to account for the zero at beginning and end.
        }
    }
}

TEST_CASE("geneticSolver throws if mutation probability is greater than 1", "[geneticSolver]")
{
    const double minDistance = 100.0;
    const double maxDistance = 500.0;
    const size_t numDepots = 1;
    const size_t numCustomers = 10;
    const size_t maxPackages = 5;
    const size_t populationSize = 10;
    const size_t generations = 10;
    const float mutationProb = 1.5;
    StartingType startingType = StartingType::ClarkeWright;

    std::vector<Point> depots = getRandomPoints(numDepots, minDistance, maxDistance);
    std::vector<Point> customers = getRandomPoints(numCustomers, minDistance, maxDistance);
    Matrix distanceMatrix = getDistanceMatrix(depots, customers);

    REQUIRE_THROWS(geneticSolver(
        distanceMatrix, maxPackages, populationSize, generations, mutationProb, startingType));
}
TEST_CASE("geneticSolver throws if maxPackages < 2", "[geneticSolver]")
{
    const double minDistance = 100.0;
    const double maxDistance = 500.0;
    const size_t numDepots = 1;
    const size_t numCustomers = 10;
    const size_t maxPackages = 0;
    const size_t populationSize = 10;
    const size_t generations = 10;
    const float mutationProb = 0.5;
    StartingType startingType = StartingType::ClarkeWright;

    std::vector<Point> depots = getRandomPoints(numDepots, minDistance, maxDistance);
    std::vector<Point> customers = getRandomPoints(numCustomers, minDistance, maxDistance);
    Matrix distanceMatrix = getDistanceMatrix(depots, customers);

    REQUIRE_THROWS(geneticSolver(
        distanceMatrix, maxPackages, populationSize, generations, mutationProb, startingType));

    REQUIRE_THROWS(geneticSolver(
        distanceMatrix, maxPackages + 1, populationSize, generations, mutationProb, startingType));
}

TEST_CASE("geneticSolver throws if distance matrix is empty", "[geneticSolver]")
{
    const double minDistance = 100.0;
    const double maxDistance = 500.0;
    const size_t numDepots = 0;
    const size_t numCustomers = 10;
    const size_t maxPackages = 5;
    const size_t populationSize = 10;
    const size_t generations = 10;
    const float mutationProb = 0.5;
    StartingType startingType = StartingType::ClarkeWright;

    std::vector<Point> depots = getRandomPoints(numDepots, minDistance, maxDistance);
    std::vector<Point> customers = getRandomPoints(numCustomers, minDistance, maxDistance);
    Matrix distanceMatrix;

    REQUIRE_THROWS(geneticSolver(
        distanceMatrix, maxPackages, populationSize, generations, mutationProb, startingType));
}