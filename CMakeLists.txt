cmake_minimum_required(VERSION 3.20)
project(vrp)

set(CMAKE_CXX_STANDARD 17)

add_executable(vrp src/main.cpp src/clarke_wright.cpp)

target_link_libraries(vrp sfml-graphics sfml-window sfml-system)
include_directories(include)
