cmake_minimum_required(VERSION 3.20)
project(vrp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_PYTHON_BINDINGS "Build Python bindings" ON)

find_package(OpenMP REQUIRED)
find_package(Catch2 3 REQUIRED)

if(BUILD_PYTHON_BINDINGS)
    find_package(pybind11 CONFIG REQUIRED)
endif()

add_library(vrp_lib STATIC
src/utils.cpp
src/clarke_wright.cpp
src/genetic_algorithm.cpp
src/create_child.cpp
src/genetic_algo_utils.cpp
src/api_solvers.cpp
)

set_target_properties(vrp_lib PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(vrp_lib PUBLIC include)
target_link_libraries(vrp_lib PUBLIC OpenMP::OpenMP_CXX)

if(BUILD_PYTHON_BINDINGS)
    pybind11_add_module(_vrp_core src/bindings.cpp)
    target_link_libraries(_vrp_core PRIVATE vrp_lib)
    install(TARGETS _vrp_core DESTINATION vrp_solver)
endif()

add_executable(vrp src/main.cpp)
target_link_libraries(vrp PRIVATE vrp_lib)

add_executable(vrp_tests
    tests/test_clarke_wright.cpp
    tests/test_genetic_algorithm.cpp
)
target_link_libraries(vrp_tests PRIVATE vrp_lib Catch2::Catch2WithMain)

enable_testing()
add_test(NAME vrp_tests COMMAND vrp_tests)
