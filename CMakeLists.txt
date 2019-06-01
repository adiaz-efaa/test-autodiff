cmake_minimum_required(VERSION 3.14)
project(autodiff)

set(CMAKE_CXX_STANDARD 17)
add_executable(test_autodiff test1.cpp)
add_executable(newton_raphson newton_raphson.cpp unused.cpp)
target_include_directories(test_autodiff PUBLIC ./include)
target_include_directories(newton_raphson PUBLIC ./include)
