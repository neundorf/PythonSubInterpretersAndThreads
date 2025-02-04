cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(PySub)

find_package(fmt REQUIRED)

find_package(Python 3.12 REQUIRED COMPONENTS Development.Embed)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(subinterp main.cpp
                         PythonInterpreter.cpp
                  )


target_link_libraries(subinterp fmt::fmt Python::Python)
