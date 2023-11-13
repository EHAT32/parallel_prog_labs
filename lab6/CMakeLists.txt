cmake_minimum_required(VERSION 3.21)
project(lab_06)
set(SOURCE_FILES 
    src/main.cpp
)
add_executable(atomic src/main.cpp)
find_package(OpenMP REQUIRED)
target_link_libraries(atomic PRIVATE  OpenMP::OpenMP_CXX)