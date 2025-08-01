Vehicle Routing Problem

## Dependencies
- **Catch2** (for testing)

### WSL Installation
sudo apt install libsfml-dev

## To Do
 - compare all the different possible starts
 - compare with the same locations
 - do something like take the best routes from A and run the clarkewright on the rest
 - maybe impliment the cross part and then try to optimize what to do after the clarke wright and nearest neighbours bc those give pretty good starts for like no computation

 - add tests for the fitness calculations
 - create a distance function for indivdual routes to avoid this routeDistancePerLocation(newRoute, distMatrix) * route.size()