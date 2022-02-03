cmake_minimum_required(VERSION 3.21)
project(tronsen_csci_310_openmp_04)

set(CMAKE_CXX_STANDARD 14)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
add_executable(tronsen_csci_310_openmp_04
        matrix_multiplication_functions.cpp
        matrix_multiplication_functions.h
        matrix_multiplication_serial.cpp)
