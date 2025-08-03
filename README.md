Vehicle Routing Problem

## Dependencies
- **Catch2** (for testing)

### WSL Installation
sudo apt install libsfml-dev

## To Do
 - add tests
 - add comparisons between algos
 - compare with the same locations

 - compare convergance of simulated anealing to genetic algo
 - add parralel version

 - add tests for the fitness calculations
 - create a distance function for indivdual routes to avoid this routeDistancePerLocation(newRoute, distMatrix) * route.size()