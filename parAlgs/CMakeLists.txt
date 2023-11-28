cmake_minimum_required(VERSION 3.21)

project(lab_08)
set(SOURCE_FILES 
    src/main.cpp
    src/calculatePi.cpp
)
add_executable(sum src/main.cpp)
add_executable(pi src/calculatePi.cpp)
find_package(OpenMP REQUIRED)
target_link_libraries(sum PRIVATE  OpenMP::OpenMP_CXX)
target_link_libraries(pi PRIVATE  OpenMP::OpenMP_CXX)