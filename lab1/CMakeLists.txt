cmake_minimum_required(VERSION 3.21)
project(sse_operations)
set(SOURCE_FILES 
    src/sqrt.cpp
    src/inner.cpp
)
add_executable(lab_01_sqrt src/sqrt.cpp)
add_executable(lab_01_inner src/inner.cpp)