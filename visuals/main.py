import csv
import matplotlib.pyplot as plt
import matplotlib.animation as animation

def get_locations(file_name):
    with open(file_name) as routes_file:
        reader = csv.reader(routes_file)
        location_coords = next(reader)
    
    locations = []
    # -1 because the row ends in a comma
    for i in range(0, len(location_coords)-1, 2):
        locations.append([float(location_coords[i]), float(location_coords[i+1])])

    return locations

def get_routes(file_name):
    all_routes = []
    current_routes = []
    with open(file_name) as routes_file:
        reader = csv.reader(routes_file)
        next(reader)

        for row in reader:
            if row[0] == "END":
                all_routes.append(current_routes)
                current_routes = []
            else:
                route = [int(location_index) for location_index in row[:-1]] # [:-1] because row ends in a comma
                current_routes.append(route)
    
    return all_routes

def plot_routes_animation(locations, all_routes, interval=1000):
    # Get ordered location coords
    x = [location[0] for location in locations]
    y = [location[1] for location in locations]

    fig, ax = plt.subplots(figsize=(10, 10))
    ax.set_xlim(0, 550)
    ax.set_ylim(0, 550)
    ax.set_aspect('equal', adjustable='box')

    def animate(frame):
        # Clear previous routes
        ax.clear()
        ax.set_xlim(0, 550)
        ax.set_ylim(0, 550)
        ax.set_aspect('equal', adjustable='box')
        
        # Plot locations
        ax.scatter(x[:1], y[:1], c="blue", s=40)
        ax.scatter(x[1:], y[1:], c="red", s=40)
        
        # Get current group of routes
        current_routes = all_routes[frame]
        
        # Plot routes for current frame
        for route in current_routes:
            for i in range(len(route[:-1])):
                start = [x[route[i]], x[route[i+1]]]
                next_point = [y[route[i]], y[route[i+1]]]
                ax.plot(start, next_point, c="black")
        
        ax.set_title(f"Iterations {frame+1}")

    # animate
    anim = animation.FuncAnimation(fig, animate, frames=len(all_routes), 
                                 interval=interval, repeat=True)
    
    plt.show()


def main():
    file = "routes.csv"
    locations = get_locations(file)
    all_routes = get_routes(file)
    plot_routes_animation(locations, all_routes)



if __name__ == "__main__":
    main()