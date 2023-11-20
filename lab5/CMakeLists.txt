cmake_minimum_required(VERSION 3.21)
project(lab_05)
set(SOURCE_FILES 
    src/main.cpp
    src/extremum.cpp
    src/reduction.cpp
    src/dotProd.cpp
)
add_executable(mp src/main.cpp)
add_executable(minmax src/extremum.cpp)
add_executable(reduction src/reduction.cpp)
add_executable(dotProd src/dotProd.cpp)
find_package(OpenMP REQUIRED)
target_link_libraries(mp PRIVATE  OpenMP::OpenMP_CXX)
target_link_libraries(minmax PRIVATE  OpenMP::OpenMP_CXX)
target_link_libraries(reduction PRIVATE  OpenMP::OpenMP_CXX)
target_link_libraries(dotProd PRIVATE  OpenMP::OpenMP_CXX)