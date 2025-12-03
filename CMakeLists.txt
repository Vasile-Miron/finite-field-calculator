cmake_minimum_required(VERSION 3.20)
project(finite_field_calculator)

add_library(ff_core INTERFACE)
target_include_directories(ff_core INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(finite_field_calculator src/main.cpp)
target_link_libraries(finite_field_calculator PRIVATE ff_core)