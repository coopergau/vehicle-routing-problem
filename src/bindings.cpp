#include "api_solvers.h"
#include "genetic_algorithm.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

// For accessing these functions in Python

PYBIND11_MODULE(_vrp_core, m)
{
    m.doc() = "Vehicle routing problem solver module for using C++ algorithms in Python";

    py::enum_<StartingType>(m, "StartingType")
        .value("ClarkeWright", StartingType::ClarkeWright)
        .value("NearestNeighbours", StartingType::NearestNeighbours)
        .value("Random", StartingType::Random)
        .value("Mixed", StartingType::Mixed)
        .export_values();

    m.def("completeSolverClarkeWright", &completeSolverClarkeWright,
          py::arg("depot_x"),
          py::arg("depot_y"),
          py::arg("customers_x"),
          py::arg("customers_y"),
          py::arg("maxPackages"),
          py::arg("exportData"),
          py::arg("fileName") = "");

    m.def("completeSolverGenetic", &completeSolverGenetic,
          py::arg("depot_x"),
          py::arg("depot_y"),
          py::arg("customers_x"),
          py::arg("customers_y"),
          py::arg("maxPackages"),
          py::arg("populationSize"),
          py::arg("generations"),
          py::arg("mutationProb"),
          py::arg("exportData"),
          py::arg("startingType") = StartingType::ClarkeWright,
          py::arg("fileName") = "");
}