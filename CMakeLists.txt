cmake_minimum_required(VERSION 3.15)
project(sparse_matrix_christianledgard)

set(CMAKE_CXX_STANDARD 17)

add_executable(sparse_matrix_christianledgard main.cpp SparceMatrix.h Node.h)