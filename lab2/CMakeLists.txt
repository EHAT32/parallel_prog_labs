cmake_minimum_required(VERSION 3.21)
project(lab_02)
set(SOURCE_FILES 
    src/matr.cpp
    src/bst_set.cpp
)
add_executable(matrix_thread src/matr.cpp)
add_executable(bst_set src/bst_set.cpp)