cmake_minimum_required (VERSION 3.22)

project (ImageEditor VERSION 1.0)


set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED True)

# set (CMAKE_CXX_FLAGS "-O3")

add_subdirectory (Editor)
add_subdirectory (Main)