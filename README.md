Vehicle Routing Problem Solver

## Table of Contents
- [Project Overview](#project-overview)
- [Features](#features)
- [Requirements](#requirements)
- [How to Run](#how-to-run)
- [Examples](#examples)

## Project Overview


## Features


## Requirements

- Python 3.x  
- OpenMP (for parellel computing)
- Catch2 (for testing in c++)

## How to Run

### Python API

1. **Clone this repository**
    ```bash
    git clone https://github.com/coopergau/vehicle-routing-problem
    cd vehicle-routing-problem
    ```

1. **Install necessary dependencies**
    ```bash
    sudo apt-get update
    sudo apt-get install -y catch2
    ```

2. **Install the API**
    ```bash
    pip install .
    ```

3. **Use the example folder (or make your own)**
    ```bash
    cd example
    ```

4. **Run the example**
    ```bash
    python3 main.py
    ```

### Raw C++ Code

1. **Clone this repository**
    ```bash
    git clone https://github.com/coopergau/vehicle-routing-problem
    cd vehicle-routing-problem
    ```

2. **Install dependencies**
    ```bash
    sudo apt-get update
    sudo apt-get install -y catch2 build-essential cmake libomp-dev
    ```

3. **Build with CMake**
    ```bash
    mkdir build
    cd build
    cmake -DBUILD_PYTHON_BINDINGS=OFF ..
    make
    ```

4. **Run the executable**
    ```bash
    ./vrp
    ```

5. **Run the tests**
    ```bash
    ./vrp_tests
    ```

## Examples



## To Do
clean
 - maybe remove the inlcudes bc of the include stuff in the cmake
 - clean #includes and comments
 - check which cmake files in the root can be removed
 
 ## Benchmark Info
 using 
const size_t numCustomers = 50;
const size_t numDepots = 1;
const size_t maxPackages = 10;
const size_t populationSize = 100;
const size_t maxGenerations = 1000;
const float mutationProb = 0.5;
const double minDistance = 100;
const double maxDistance = 1000;
const double centerCoords = 550;

ran loop 10 times twice and got average:

Sequential: 12641ms
Parallel: 3798.2ms
Speedup: 3.32816x

Sequential: 12672.5ms
Parallel: 3746.5ms
Speedup: 3.38249x