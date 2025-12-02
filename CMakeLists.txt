cmake_minimum_required(VERSION 4.0)
project(finite_field_calculator)

include_directories(include)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(finite_field_calculator src/main.cpp
        include/finiteFieldElement.hpp)
