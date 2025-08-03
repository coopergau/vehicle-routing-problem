Vehicle Routing Problem

## Dependencies
- **Catch2** (testing)
- **OpenMP** (parallel computing)

### WSL Installation
sudo apt install libsfml-dev

## To Do
 - add tests
 - add comparisons between algos
 - compare with the same locations

 - add tests for the fitness calculations
 - create a distance function for indivdual routes to avoid this routeDistancePerLocation(newRoute, distMatrix) * route.size()