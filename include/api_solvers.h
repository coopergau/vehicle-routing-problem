#ifndef API_SOLVERS_H
#define API_SOLVERS_H

#include <vector>
#include "utils.h"
#include "clarke_wright.h"
#include "genetic_algorithm.h"

std::vector<std::vector<std::vector<int>>> completeSolverClarkeWright(
    const double &depot_x,
    const double &depot_y,
    const std::vector<double> &customers_x,
    const std::vector<double> &customers_y,
    const size_t maxPackages,
    const bool exportData,
    const std::string &filename = "");

std::vector<std::vector<std::vector<int>>> completeSolverGenetic(
    const double &depot_x,
    const double &depot_y,
    const std::vector<double> &customers_x,
    const std::vector<double> &customers_y,
    const size_t maxPackages,
    const size_t populationSize,
    const size_t generations,
    const float mutationProb,
    const bool exportData,
    const StartingType startingType = StartingType::ClarkeWright,
    const std::string &filename = "");

#endif