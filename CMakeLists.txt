cmake_minimum_required(VERSION 3.4)
project(AshVM)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")

set(SOURCE_FILES main.cpp ash/vm.cpp ash/vm.hpp ash/fileimport.h ash/fileimport.cpp)
add_executable(AshVM ${SOURCE_FILES})