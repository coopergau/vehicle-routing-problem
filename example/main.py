#Example use in python
import vrp_solver


def main():
    file = "routes.csv"
    num_customers = 80
    max_packages = 10
    population_size = 100
    generations = 1000
    mutation_prob = 0.5
    depot_coords = 100

    locations_x = [638, 471.562, 708.806, 797.317, 299.051, 533.925, 453.046, 885.210, 695.615, 314.599, 752.461, 988.943, 626.947, 930.201, 599.925, 189.84, 275.959, 457.436, 931.662, 948.443, 419.236, 869.817, 947.822, 547.613, 770.531, 219.516, 153.438, 318.300, 403.169, 522.098, 794.861, 132.592, 243.351, 981.814, 359]
    locations_y = [504, 452.871, 463.492, 778.824, 947.275, 940.909, 702.878, 831.618, 570.149, 342.502, 264.979, 530.139, 240.031, 146.053, 575.459, 865.74, 469.273, 202.455, 179.411, 772.261, 764.169, 672.552, 970.377, 286.836, 186.545, 278.056, 881.281, 623.291, 411.491, 561.256, 964.085, 170.396, 138.856, 916.693, 557]

    clarkeWrightSolution = vrp_solver.completeSolverClarkeWright(
        depot_coords,
        depot_coords,
        locations_x,
        locations_y,
        max_packages,
        True,
        file)
    
    geneticSolution = vrp_solver.completeSolverGenetic(
        depot_coords,
        depot_coords,
        locations_x,
        locations_y,
        max_packages,
        population_size,
        generations,
        mutation_prob,
        True,
        vrp_solver.StartingType.ClarkeWright,
        file)

    locations = vrp_solver.get_locations("routes.csv")
    routes = vrp_solver.get_routes("routes.csv")
    vrp_solver.plot_routes_animation(locations, routes, interval=100)

if __name__ == "__main__":
    main()