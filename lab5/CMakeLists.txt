cmake_minimum_required(VERSION 3.21)
project(lab_05)
set(SOURCE_FILES 
    src/main.cpp
)
add_executable(mp src/main.cpp)
find_package(OpenMP REQUIRED)
target_link_libraries(mp PRIVATE  OpenMP::OpenMP_CXX)