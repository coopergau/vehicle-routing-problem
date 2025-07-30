cmake_minimum_required(VERSION 3.20)
project(vrp)

set(CMAKE_CXX_STANDARD 17)

add_executable(vrp src/main.cpp src/utils.cpp src/clarke_wright.cpp src/genetic_algorithm.cpp src/genetic_algo_utils.cpp)

target_link_libraries(vrp sfml-graphics sfml-window sfml-system)
include_directories(include)

# Stuff for tests
find_package(Catch2 3 REQUIRED)

add_executable(vrp_tests tests/test_clarke_wright.cpp src/utils.cpp src/clarke_wright.cpp)
target_link_libraries(vrp_tests PRIVATE Catch2::Catch2WithMain)

enable_testing()
add_test(NAME ClarkeWright_Tests COMMAND vrp_tests)
