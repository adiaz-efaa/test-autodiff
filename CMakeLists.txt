cmake_minimum_required(VERSION 3.14)
project(autodiff)

set(CMAKE_CXX_STANDARD 17)
add_executable(test_autodiff main.cpp)
target_include_directories(test_autodiff PUBLIC ./include)
