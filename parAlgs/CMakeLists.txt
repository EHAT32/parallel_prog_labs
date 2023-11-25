cmake_minimum_required(VERSION 3.21)

project(lab_08)
set(SOURCE_FILES 
    src/main.cpp
)
add_executable(sum src/main.cpp)
find_package(OpenMP REQUIRED)
target_link_libraries(sum PRIVATE  OpenMP::OpenMP_CXX)