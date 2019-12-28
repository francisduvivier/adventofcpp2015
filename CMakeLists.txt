cmake_minimum_required(VERSION 3.15)
project(adventofcode2015)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        adventofcode2015
#        day1/day1.cpp
        day2/day2.cpp
        util/processing.h util/processing.cpp
#        day3/day3.cpp
)