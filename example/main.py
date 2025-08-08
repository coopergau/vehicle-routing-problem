#Example use in python
import vrp_solver
import random
import matplotlib.pyplot as plt
    
'''
Valid starting types:
vrp_solver.StartingType.Random
vrp_solver.StartingType.ClarkeWright
vrp_solver.StartingType.NearestNeighbours
vrp_solver.StartingType.Mixed
'''

def main():
    file = "routes.csv"
    num_customers = 50
    max_packages = 10
    population_size = 100
    generations = 1000
    mutation_prob = 0.5

    depot_x = random.uniform(100, 1000)
    depot_y = random.uniform(100, 1000)
    locations_x = [random.uniform(100, 1000) for _ in range(num_customers)]
    locations_y = [random.uniform(100, 1000) for _ in range(num_customers)]

    clarkeWrightSolution = vrp_solver.completeSolverClarkeWright(
        depot_x,
        depot_y,
        locations_x,
        locations_y,
        max_packages,
        True,
        file)
    
    '''
    geneticSolution = vrp_solver.completeSolverGenetic(
        depot_x,
        depot_y,
        locations_x,
        locations_y,
        max_packages,
        population_size,
        generations,
        mutation_prob,
        True,
        vrp_solver.StartingType.Mixed,
        file)
    '''

    locations = vrp_solver.get_locations(file)
    routes = vrp_solver.get_routes(file)
    anim = vrp_solver.plot_routes_animation(locations, routes, interval=100)
    # anim.save("ClarkeWright.gif", writer='pillow', fps=10) # Uncomment to save the animation as a gif
    plt.show()

if __name__ == "__main__":
    main()