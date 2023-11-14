cmake_minimum_required(VERSION 3.21)
set(TBB_ROOT C:/cpp/vcpkg/packages/tbb_x64-windows)
project(lab_07)
set(SOURCE_FILES 
    src/main.cpp
)
find_package(TBB REQUIRED)
add_executable(graph src/main.cpp)
target_link_libraries(graph TBB::tbb)

