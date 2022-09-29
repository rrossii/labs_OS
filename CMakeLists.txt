cmake_minimum_required(VERSION 3.23)
project(lab3_OS)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab3_OS main.cpp textGenerator.cpp)
add_executable(generator textGenerator.cpp)
