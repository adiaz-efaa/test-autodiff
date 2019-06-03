cmake_minimum_required(VERSION 3.14)
project(autodiff)

set(CMAKE_CXX_STANDARD 17)
add_executable(test_autodiff test1.cpp)
add_executable(newton_raphson newton_raphson.cpp)
add_executable(bootstrapping_1 bootstrapping_1.cpp)
add_executable(bootstrapping_2 bootstrapping_2.cpp)

target_include_directories(test_autodiff PUBLIC ./include)
target_include_directories(newton_raphson PUBLIC ./include)
target_include_directories(bootstrapping_1 PUBLIC ./include)
target_include_directories(bootstrapping_2 PUBLIC ./include)
