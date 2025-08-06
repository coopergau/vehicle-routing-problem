cmake_minimum_required(VERSION 3.20)
project(vrp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenMP REQUIRED)
find_package(Catch2 3 REQUIRED)

add_library(vrp_lib
    src/utils.cpp
    src/clarke_wright.cpp
    src/genetic_algorithm.cpp
    src/create_child.cpp
    src/genetic_algo_utils.cpp
)

target_include_directories(vrp_lib PUBLIC include)

add_executable(vrp src/main.cpp)
target_link_libraries(vrp PRIVATE vrp_lib OpenMP::OpenMP_CXX)

add_executable(vrp_tests tests/test_clarke_wright.cpp tests/test_genetic_algorithm.cpp)
target_link_libraries(vrp_tests PRIVATE vrp_lib Catch2::Catch2WithMain OpenMP::OpenMP_CXX)

enable_testing()
add_test(NAME vrp_tests COMMAND vrp_tests)
