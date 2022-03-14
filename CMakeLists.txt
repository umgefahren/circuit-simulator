cmake_minimum_required(VERSION 3.21)
project(circuit_simulator)

set(CMAKE_CXX_STANDARD 20)


include(FetchContent)

# FetchContent_Declare(
#         highway
#         GIT_REPOSITORY https://github.com/google/highway.git
#         GIT_TAG 0.16.0
# )

# FetchContent_MakeAvailable(highway)

add_executable(circuit_simulator main.cpp simd/basic_test.cpp simd/basic_test.h gates.cpp gates.h parser.cpp parser.h visualizer.cpp visualizer.h)