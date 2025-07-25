import pandas as pd
import matplotlib.pyplot as plt

def get_locations(file_name):
    routes_df = pd.read_csv(file_name, header=None)

    location_coords = list(routes_df.iloc[0])[0:-1]
    locations = []
    for i in range(0, len(location_coords), 2):
        locations.append([location_coords[i], location_coords[i+1]])

    return locations

def get_routes(file_name):
    routes_df = pd.read_csv(file_name, header=None, skiprows=1)

    routes = []
    for _, row in routes_df.iterrows():
        routes.append(list(row.dropna().astype(int))[0:-1])
    
    return routes

def plot_routes(locations, routes):
    # Plot locations
    x = [location[0] for location in locations]
    y = [location[1] for location in locations]

    plt.figure(figsize=(10, 10))
    plt.scatter(x[:1], y[:1], c="blue", s=40)
    plt.scatter(x[1:], y[1:], c="red", s=40)
    plt.xlim(0, 550)
    plt.ylim(0, 550)
    plt.gca().set_aspect('equal', adjustable='box')
    plt.show()

    # Plot routes



def main():
    file = "routes.csv"
    locations = get_locations(file)
    routes = get_routes(file)
    plot_routes(locations, routes)




if __name__ == "__main__":
    main()