Vehicle Routing Problem

## Dependencies
- **Catch2** (for testing)

### WSL Installation
sudo apt install libsfml-dev

## To Do
 - compare all the different possible starts
 - compare with the same locations

 - add tests for the fitness calculations
 - create a distance function for indivdual routes to avoid this routeDistancePerLocation(newRoute, distMatrix) * route.size()